# Phase 2 구현 요약

## 개요

**목표**: 전역/지역 변수, 대입 연산, 후위 증감 연산자 코드 생성
**테스트 파일**: t2.c, t3.c, t4.c
**구현 순서**: Step 2-1 → Step 2-2 → Step 2-4 → Step 2-3

---

## Step 2-1: 전역 변수 offset 계산

### 목표
전역 변수에 `Lglob+offset` 형태의 주소 할당

### 구현 사항

**1. subc.h - decl 구조체 확장**
```c
int offset;      /* VAR/CONST: offset (global: Lglob+offset, local: fp+offset) */
int is_global;   /* VAR/CONST: 1=global, 0=local */
```

**2. symtab.c - size_in_words() 함수 추가**
```c
int size_in_words(struct decl *d) {
    if (d == NULL || d->size <= 0) return 1;  /* 최소 1 word */
    return (d->size + 3) / 4;  /* bytes → words (올림) */
}
```

**3. subc.y - ext_def 규칙에서 offset 할당**
```c
vardecl->offset = global_words;
vardecl->is_global = 1;
global_words += size_in_words(vardecl);
```

### 핵심 포인트
- `global_words`는 전역 변수 총 크기 (word 단위)
- 프로그램 끝에서 `Lglob. data <global_words>` 출력

---

## Step 2-2: 함수 뼈대 (main)

### 목표
main 함수의 프롤로그/에필로그 생성

### 구현 사항

**1. Startup 코드** (program 규칙)
```asm
push_const EXIT
push_reg fp
push_reg sp
pop_reg fp
jump main
EXIT:
exit
```

**2. 함수 레이블 구조**
```asm
main:           # 함수 진입점
main_start:     # 본문 시작
  ...           # 함수 body
main_final:     # 에필로그
  push_reg fp
  pop_reg sp
  pop_reg fp
  pop_reg pc
main_end:       # 함수 종료
```

**3. 플래그 시스템**
```c
static char *current_func_name = NULL;     /* 현재 함수명 */
static int pending_function_body = 0;      /* 다음 compound_stmt가 함수 body */
static int in_function_body = 0;           /* 현재 함수 body 처리 중 */
```

### 핵심 포인트
- `pending_function_body`: func_decl에서 설정, compound_stmt에서 확인
- `in_function_body`: compound_stmt 내에서만 유효, 중첩 블록 구분에 사용

---

## Step 2-4: L-value/R-value + 대입 + 후위증감 ⭐

### 목표
표현식의 코드 생성 - 특히 후위 증감 연산자

### 핵심 개념: L-value vs R-value

| 구분 | 의미 | 스택에 push되는 것 |
|------|------|-------------------|
| L-value | 대입 가능한 위치 | **주소** |
| R-value | 값 자체 | **값** |

```c
x = 3;
// x는 L-value (주소가 필요)
// 3은 R-value (값 자체)
```

### VM 명령어 스택 효과

| 명령어 | 스택 변화 | 설명 |
|--------|----------|------|
| `assign` | [addr, val] → [] | mem[addr]=val, **둘 다 pop** |
| `fetch` | [addr] → [mem[addr]] | 주소를 값으로 변환 |
| `push_reg sp; fetch` | [x] → [x, x] | **스택 top 복제 패턴** |

### 구현 1: 변수 접근 (unary: ID)

```c
/* 전역 변수: L-value (주소) push */
if (d->is_global) {
    emit("push_const Lglob+%d", d->offset);
} else {
    /* 지역 변수: fp + offset */
    emit("push_reg fp");
    emit("push_const %d", d->offset);
    emit("add");
}
```

### 구현 2: 대입 연산 (assignment)

**문제**: `assign`은 [addr, val] → []로 둘 다 pop하므로, 대입 결과값을 얻으려면 주소를 미리 복제해야 함

**해결**: Mid-rule action으로 LHS 주소 복제

```yacc
assignment
  : unary
    {
      /* Mid-rule: LHS 주소 복제 */
      emit("push_reg sp");
      emit("fetch");
    }
    '=' assignment
  {
    /* RHS가 L-value면 R-value로 변환 */
    if ($4.is_lvalue) {
      emit("fetch");
    }
    emit("assign");
    emit("fetch");  /* 대입 결과값 */
  }
```

**스택 트레이스 (`x = 3`)**:
```
1. unary(x) 파싱      : [addr]
2. mid-rule 복제      : [addr, addr]
3. assignment(3) 파싱 : [addr, addr, 3]
4. assign             : [addr]        (mem[addr]=3)
5. fetch              : [3]           (대입 결과값)
```

---

## ⭐⭐ 후위 증감 연산자 (x++) 구현 상세 ⭐⭐

### 요구사항
- `x++`의 결과값은 **증가 전의 값 (old value)**
- 변수 x는 **1 증가된 값으로 갱신**

### 핵심 도전
1. `assign`이 스택을 모두 소비하므로, 결과값을 위한 주소 보존 필요
2. 결과값은 old value여야 하지만, 저장되는 값은 new value

### 해결 패턴: Triple Address Copy

**아이디어**: 주소를 3번 복제하여 각각의 용도로 사용
1. 첫 번째 복제: assign 후 결과값 fetch용
2. 두 번째 복제: assign의 주소 인자
3. 세 번째 복제: fetch로 old value 읽기용

### 스택 트레이스 (x++ 완전 분석)

**초기 상태**: unary(x) 파싱 후 스택에 [addr]

```
단계  명령어           스택 상태                설명
────────────────────────────────────────────────────────────────
 0   (unary 파싱)     [addr]                  x의 주소
 1   push_reg sp      [addr, &top]            sp 값 push
 2   fetch            [addr, addr]            스택 top 복제 (1차)
 3   push_reg sp      [addr, addr, &top]      sp 값 push
 4   fetch            [addr, addr, addr]      스택 top 복제 (2차)
 5   fetch            [addr, addr, old]       old value 읽기
 6   push_const 1     [addr, addr, old, 1]    상수 1 push
 7   add              [addr, addr, new]       new = old + 1
 8   assign           [addr]                  mem[addr] = new
 9   fetch            [new]                   갱신된 값 읽기
10   push_const 1     [new, 1]                상수 1 push
11   sub              [old]                   old = new - 1 (결과값)
────────────────────────────────────────────────────────────────
최종: 스택에 old value, 메모리에 new value
```

### 구현 코드

```c
| unary INCOP
{
  /* Post-increment: result = old value
   * Stack before: [addr]
   * Pattern: dup addr twice, fetch old, add 1, assign, fetch new, sub 1
   */
  emit("push_reg sp");
  emit("fetch");      /* [addr, addr] */
  emit("push_reg sp");
  emit("fetch");      /* [addr, addr, addr] */
  emit("fetch");      /* [addr, addr, old] */
  emit("push_const 1");
  emit("add");        /* [addr, addr, new] */
  emit("assign");     /* [addr] */
  emit("fetch");      /* [new] */
  emit("push_const 1");
  emit("sub");        /* [old] - 최종 결과 */

  /* Semantic analysis */
  $$.type = $1.type;
  $$.is_lvalue = 0;  /* result is r-value */
  ...
}
```

### 후위 감소 (x--) 구현

동일한 패턴, `add`↔`sub` 교환:

```c
| unary DECOP
{
  emit("push_reg sp");
  emit("fetch");
  emit("push_reg sp");
  emit("fetch");
  emit("fetch");      /* [addr, addr, old] */
  emit("push_const 1");
  emit("sub");        /* [addr, addr, new] - 감소 */
  emit("assign");     /* [addr] */
  emit("fetch");      /* [new] */
  emit("push_const 1");
  emit("add");        /* [old] - 복원 */
}
```

### 왜 이 패턴인가?

**대안 1**: old value를 먼저 저장하고 나중에 꺼내기
- 문제: 임시 저장소 필요, 스택 관리 복잡

**대안 2**: new value에서 1을 빼서 old value 계산
- 장점: 추가 메모리 불필요, 스택만으로 해결
- 이 방식 채택!

### 검증

**t3.c**:
```c
int x;
int y;
int main() {
    x = 3;
    y = x++;  // y = 3, x = 4
}
```

**생성된 코드 (x++ 부분)**:
```asm
push_const Lglob+1      ; x의 주소
push_reg sp
fetch                   ; [addr, addr]
push_reg sp
fetch                   ; [addr, addr, addr]
fetch                   ; [addr, addr, 3]
push_const 1
add                     ; [addr, addr, 4]
assign                  ; [addr], x=4
fetch                   ; [4]
push_const 1
sub                     ; [3] - y에 대입될 값
```

---

## Step 2-3: 지역 변수 선언

### 목표
함수 내 지역 변수 공간 할당 및 접근

### 스택 프레임 레이아웃
```
[fp+0]  : old_fp (startup에서 push)
[fp+1]  : local_0
[fp+2]  : local_1
...
```

### 구현 사항

**1. local_offset 변수**
```c
static int local_offset = 0;
```

**2. compound_stmt에서 초기화**
```c
if (pending_function_body) {
    in_function_body = 1;
    pending_function_body = 0;
    emit_label(start_label);
}
if (in_function_body) {
    local_offset = 1;  /* fp+1부터 시작 */
}
```

**3. def_list 후 shift_sp 발행**
```c
if (in_function_body) {
    int num_locals = local_offset - 1;
    if (num_locals > 0) {
        emit("shift_sp %d", num_locals);
    }
}
```

**4. def 규칙에서 offset 할당**
```c
vardecl->offset = local_offset;
vardecl->is_global = 0;
local_offset += size_in_words(vardecl);
```

### 명시적 가정 (Phase 5 전까지)

1. **로컬 회수**: 에필로그(`push_reg fp; pop_reg sp`)가 sp를 fp로 복구
2. **스코프 범위**: 함수 최상위 블록의 지역 선언만 대상, 중첩 블록은 Phase 5에서 확장

---

## 테스트 결과

| 파일 | 내용 | 결과 |
|------|------|------|
| t2.c | 전역 변수 대입 (`y = x`) | ✓ pass |
| t3.c | 전역 변수 + 후위증감 (`y = x++`) | ✓ pass |
| t4.c | 지역 변수 + 후위증감 | ✓ pass |
| shadow_test.c | 전역/지역 shadowing | ✓ pass |

---

## 핵심 학습 포인트

1. **스택 top 복제 패턴**: `push_reg sp; fetch`
2. **assign 스택 효과**: [addr, val] → [] (둘 다 소비)
3. **Mid-rule action**: bison에서 중간에 코드 삽입
4. **L-value/R-value 구분**: 주소 vs 값, fetch로 변환
5. **후위 증감 패턴**: Triple copy + 역산으로 old value 복원
