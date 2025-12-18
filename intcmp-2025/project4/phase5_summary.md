# Phase 5: 제어 흐름 코드 생성 요약

## 1. 개요

Phase 5에서는 다음 제어 흐름 구문에 대한 코드 생성을 구현한다:
- if / if-else 문
- while 문
- for 문
- break / continue 문

## 2. VM 명령어 특성 (중요!)

### 2.1 분기 명령어의 스택 효과

```
branch_false <label>
- 스택 top을 POP하여 검사
- 값이 0이면 label로 점프
- Stack effect: S, cond -> S (조건값 소비됨)

branch_true <label>
- 스택 top을 POP하여 검사
- 값이 0이 아니면 label로 점프
- Stack effect: S, cond -> S (조건값 소비됨)

jump <label>
- 무조건 label로 점프
- Stack effect: 없음
```

**핵심**: `branch_false`와 `branch_true`는 조건값을 자동으로 pop하므로, 추가적인 `shift_sp -1`이 **불필요**하다.

### 2.2 스택 방향

```
스택이 **위로** 자람 (PUSH: sp++, POP: sp--)
- shift_sp n: sp += n
  - 양수: 스택 확장 (공간 확보)
  - 음수: 스택 축소 (값 버림)
```

---

## 3. 라벨 스택 구조

### 3.1 if/else용 라벨 스택

```c
static char *if_label_stack[100];
static int if_label_top = 0;
```

### 3.2 루프용 라벨 스택 (while, for, break, continue 공용)

```c
struct loop_labels {
    char *cond_label;      /* 조건 검사 위치 (while: begin, for: cond) */
    char *continue_label;  /* continue 타겟 (while: cond, for: update) */
    char *break_label;     /* break 타겟 (루프 종료점) */
    char *body_label;      /* for 전용: body 시작 (update 건너뛰기용) */
};
static struct loop_labels loop_stack[100];
static int loop_stack_top = 0;
```

**설계 의도**:
- `continue_label`은 while과 for에서 다른 위치를 가리킴
  - while: 조건 검사 위치 (cond_label과 동일)
  - for: update 식 위치
- `body_label`은 for문 전용으로, 첫 번째 iteration에서 update를 건너뛰기 위해 사용

---

## 4. if / if-else 문

### 4.1 코드 생성 템플릿

#### 단순 if문
```asm
<expr 평가>           ; 조건값이 스택 top에
branch_false Lelse_N  ; 0이면 skip (조건값 pop됨)
<stmt>
Lelse_N:
```

#### if-else문
```asm
<expr 평가>
branch_false Lelse_N  ; 0이면 else로
<stmt1>
jump Lend_N           ; else 건너뛰기
Lelse_N:
<stmt2>
Lend_N:
```

### 4.2 구현 (subc.y)

```yacc
/* if 문 접두사 - reduce-reduce 충돌 해결을 위해 별도 비단말로 분리 */
if_head
  : IF '(' expr ')'
    {
      char *label = new_label("Lelse_");
      if_label_stack[if_label_top++] = label;
      emit("branch_false %s", label);
    }
  ;

/* 단순 if */
| if_head stmt %prec LOWER_THAN_ELSE
    {
      emit_label(if_label_stack[--if_label_top]);
    }

/* if-else */
| if_head stmt ELSE
    {
      char *end_label = new_label("Lend_");
      emit("jump %s", end_label);
      emit_label(if_label_stack[--if_label_top]);
      if_label_stack[if_label_top++] = end_label;
    }
    stmt
    {
      emit_label(if_label_stack[--if_label_top]);
    }
```

---

## 5. while 문 (상세)

### 5.1 코드 생성 템플릿

```asm
Lwhile_N:             ; 루프 시작 (continue 타겟)
<cond_expr>           ; 조건 평가, 결과가 스택 top에
branch_false Lend_N   ; 조건 거짓이면 종료 (break 타겟)
<body_stmt>           ; 본문 실행
jump Lwhile_N         ; 조건 검사로 돌아감
Lend_N:               ; 루프 종료
```

### 5.2 실행 흐름 다이어그램

```
┌─────────────────────────────────────────────────────────┐
│                    while (expr) stmt                     │
└─────────────────────────────────────────────────────────┘

                         │
                         ▼
              ┌──────────────────────┐
              │    Lwhile_N:         │ ◄─────────────────┐
              │    (조건 검사 시작)   │                   │
              └──────────────────────┘                   │
                         │                               │
                         ▼                               │
              ┌──────────────────────┐                   │
              │    <expr 평가>       │                   │
              │    결과: 스택 top    │                   │
              └──────────────────────┘                   │
                         │                               │
                         ▼                               │
              ┌──────────────────────┐                   │
              │  branch_false Lend_N │                   │
              │  (조건값 pop 후 검사)│                   │
              └──────────────────────┘                   │
                    │           │                        │
           조건 참  │           │ 조건 거짓              │
                    ▼           │                        │
              ┌─────────────┐   │                        │
              │  <body_stmt>│   │                        │
              │  (본문 실행)│   │                        │
              └─────────────┘   │                        │
                    │           │                        │
                    ▼           │                        │
              ┌─────────────┐   │                        │
              │jump Lwhile_N│───┼────────────────────────┘
              └─────────────┘   │
                                │
                                ▼
              ┌──────────────────────┐
              │      Lend_N:         │
              │    (루프 종료)       │
              └──────────────────────┘
```

### 5.3 구현 상세 (subc.y)

#### while_head 규칙

```yacc
while_head
  : WHILE '('
    {
      /* Step 1: 라벨 생성 */
      char *begin_label = new_label("Lwhile_");
      char *end_label = new_label("Lend_");

      /* Step 2: loop_stack에 저장 */
      loop_stack[loop_stack_top].cond_label = begin_label;
      loop_stack[loop_stack_top].continue_label = begin_label;  /* while: continue→조건 */
      loop_stack[loop_stack_top].break_label = end_label;
      loop_stack[loop_stack_top].body_label = NULL;  /* while은 body_label 미사용 */
      loop_stack_top++;

      /* Step 3: 루프 시작 라벨 emit */
      emit_label(begin_label);
    }
    expr ')'
    {
      /* Step 4: 조건 평가 후 branch_false */
      /* expr이 평가되어 스택 top에 조건값이 있음 */
      emit("branch_false %s", loop_stack[loop_stack_top-1].break_label);
    }
  ;
```

#### stmt 규칙 (while 부분)

```yacc
| while_head stmt
    {
      /* Step 5: 루프 끝 - jump back + end label */
      emit("jump %s", loop_stack[loop_stack_top-1].continue_label);
      emit_label(loop_stack[loop_stack_top-1].break_label);
      loop_stack_top--;
      REDUCE("stmt->WHILE '(' expr ')' stmt");
    }
```

### 5.4 코드 생성 워크플로우 (단계별)

**예제 코드**:
```c
int main() {
    int i;
    i = 0;
    while (i < 3) {
        write_int(i);
        i = i + 1;
    }
    return 0;
}
```

**파싱 및 코드 생성 순서**:

| 단계 | 파싱 요소 | 액션 | emit되는 코드 |
|------|-----------|------|---------------|
| 1 | `WHILE` | while_head 진입 | - |
| 2 | `'('` | 라벨 생성, 스택 push | `Lwhile_0:` |
| 3 | `i < 3` (expr) | 조건식 평가 코드 생성 | `push_reg fp` `push_const 1` `add` `fetch` `push_const 3` `less` |
| 4 | `')'` | branch_false emit | `branch_false Lend_1` |
| 5 | `{ ... }` (stmt) | 본문 코드 생성 | `push_const ...` `write_int` `...` |
| 6 | stmt 완료 | jump + end label | `jump Lwhile_0` `Lend_1:` |

**생성된 IR 코드**:
```asm
; i = 0;
push_reg fp
push_const 1
add
push_const 0
assign
shift_sp -1

; while (i < 3)
Lwhile_0:
push_reg fp
push_const 1
add
fetch
push_const 3
less
branch_false Lend_1

; { write_int(i); i = i + 1; }
push_reg fp
push_const 1
add
fetch
write_int
push_reg fp
push_const 1
add
push_reg fp
push_const 1
add
fetch
push_const 1
add
assign
fetch
shift_sp -1

; 루프 끝
jump Lwhile_0
Lend_1:
```

### 5.5 break/continue와의 연동

```
while 문에서:
- break → jump Lend_N (break_label)
- continue → jump Lwhile_N (continue_label = cond_label)
```

**예제: while + break + continue**
```c
while (x <= 10) {
    if (x == 5) { x++; continue; }  // continue → Lwhile_N
    if (x == 8) { break; }          // break → Lend_N
    z = z + x++;
}
```

---

## 6. for 문 (상세)

### 6.1 핵심 문제: 파싱 순서 vs 실행 순서

**C 언어의 for문 실행 순서**:
```
init → cond → body → update → cond → body → update → ...
```

**문법적 파싱 순서**:
```
FOR '(' init ';' cond ';' update ')' body
         1        2        3          4
```

**문제점**: update(3번)가 body(4번)보다 먼저 파싱되므로, syntax-directed 방식으로는 update 코드가 body 코드보다 먼저 emit된다.

**해결책**: `jump Lbody`를 사용하여 첫 번째 iteration에서 update를 건너뛴다.

### 6.2 코드 생성 템플릿

```asm
<init_expr>           ; 초기화 (있으면)
shift_sp -1           ; init 결과 버림 (있으면)

Lcond_N:              ; 조건 검사 시작
<cond_expr>           ; 조건 (없으면 push_const 1)
branch_false Lend_N   ; 조건 거짓이면 종료

jump Lbody_N          ; ★ 첫 iteration: update 건너뛰기

Lupdate_N:            ; update 시작 (continue 타겟)
<update_expr>         ; 업데이트 (있으면)
shift_sp -1           ; update 결과 버림 (있으면)
jump Lcond_N          ; 조건 검사로

Lbody_N:              ; 본문 시작
<body_stmt>           ; 본문 실행
jump Lupdate_N        ; update로

Lend_N:               ; 루프 종료 (break 타겟)
```

### 6.3 실행 흐름 다이어그램

```
┌─────────────────────────────────────────────────────────────────────────────┐
│           for (init_expr; cond_expr; update_expr) body_stmt                 │
└─────────────────────────────────────────────────────────────────────────────┘

                              │
                              ▼
                   ┌─────────────────────┐
                   │    <init_expr>      │
                   │    shift_sp -1      │ (init 결과 버림)
                   └─────────────────────┘
                              │
                              ▼
                   ┌─────────────────────┐
                   │     Lcond_N:        │ ◄───────────────────────────┐
                   │   (조건 검사)       │                              │
                   └─────────────────────┘                              │
                              │                                         │
                              ▼                                         │
                   ┌─────────────────────┐                              │
                   │   <cond_expr>       │                              │
                   │   (없으면 push 1)   │                              │
                   └─────────────────────┘                              │
                              │                                         │
                              ▼                                         │
                   ┌─────────────────────┐                              │
                   │ branch_false Lend_N │                              │
                   └─────────────────────┘                              │
                         │         │                                    │
                조건 참  │         │ 조건 거짓                          │
                         ▼         │                                    │
                   ┌───────────┐   │                                    │
                   │jump Lbody │───┼──────────────┐                     │
                   └───────────┘   │              │                     │
                                   │              │ (첫 iteration)      │
                                   │              │                     │
                   ┌───────────────┴───┐          │                     │
                   │    Lupdate_N:     │ ◄────────┼─────────────┐       │
                   │  (continue 타겟)  │          │             │       │
                   └───────────────────┘          │             │       │
                              │                   │             │       │
                              ▼                   │             │       │
                   ┌─────────────────────┐        │             │       │
                   │   <update_expr>     │        │             │       │
                   │   shift_sp -1       │        │             │       │
                   └─────────────────────┘        │             │       │
                              │                   │             │       │
                              ▼                   │             │       │
                   ┌─────────────────────┐        │             │       │
                   │   jump Lcond_N      │────────┼─────────────┼───────┘
                   └─────────────────────┘        │             │
                                                  │             │
                   ┌─────────────────────┐        │             │
                   │     Lbody_N:        │ ◄──────┘             │
                   │   (본문 시작)       │                      │
                   └─────────────────────┘                      │
                              │                                 │
                              ▼                                 │
                   ┌─────────────────────┐                      │
                   │    <body_stmt>      │                      │
                   └─────────────────────┘                      │
                              │                                 │
                              ▼                                 │
                   ┌─────────────────────┐                      │
                   │  jump Lupdate_N     │──────────────────────┘
                   └─────────────────────┘

                         │ (조건 거짓 시)
                         ▼
                   ┌─────────────────────┐
                   │      Lend_N:        │
                   │   (break 타겟)      │
                   └─────────────────────┘
```

### 6.4 구현 상세 (subc.y)

#### for_head 규칙

```yacc
for_head
  : FOR '(' expr_e ';'
    {
      /* Step 1: init expr 결과 버림 (있으면) */
      if ($3.type != NULL) {
        emit("shift_sp -1");
      }

      /* Step 2: 라벨 4개 생성 */
      char *cond_label = new_label("Lcond_");
      char *update_label = new_label("Lupdate_");
      char *body_label = new_label("Lbody_");
      char *end_label = new_label("Lend_");

      /* Step 3: loop_stack에 저장 */
      loop_stack[loop_stack_top].cond_label = cond_label;
      loop_stack[loop_stack_top].continue_label = update_label;  /* for: continue→update */
      loop_stack[loop_stack_top].break_label = end_label;
      loop_stack[loop_stack_top].body_label = body_label;
      loop_stack_top++;

      /* Step 4: cond 라벨 emit */
      emit_label(cond_label);
    }
    expr_e ';'
    {
      /* Step 5: cond expr 처리 (없으면 push_const 1 = 무한루프) */
      if ($6.type == NULL) {
        emit("push_const 1");
      }

      /* Step 6: branch_false → end */
      emit("branch_false %s", loop_stack[loop_stack_top-1].break_label);

      /* Step 7: jump → body (첫 iteration에서 update 건너뛰기) */
      emit("jump %s", loop_stack[loop_stack_top-1].body_label);

      /* Step 8: update 라벨 emit */
      emit_label(loop_stack[loop_stack_top-1].continue_label);
    }
  ;
```

#### stmt 규칙 (for 부분)

```yacc
| for_head expr_e
    {
      /* Step 9: update expr 결과 버림 (있으면) */
      if ($2.type != NULL) {
        emit("shift_sp -1");
      }

      /* Step 10: jump → cond */
      emit("jump %s", loop_stack[loop_stack_top-1].cond_label);

      /* Step 11: body 라벨 emit */
      emit_label(loop_stack[loop_stack_top-1].body_label);
    }
    ')' stmt
    {
      /* Step 12: jump → update */
      emit("jump %s", loop_stack[loop_stack_top-1].continue_label);

      /* Step 13: end 라벨 emit */
      emit_label(loop_stack[loop_stack_top-1].break_label);

      /* Step 14: loop_stack pop */
      loop_stack_top--;
      REDUCE("stmt->FOR '(' expr_e ';' expr_e ';' expr_e ')' stmt");
    }
```

### 6.5 코드 생성 워크플로우 (단계별)

**예제 코드**:
```c
int main() {
    int i;
    for (i = 0; i < 3; i = i + 1) {
        write_int(i);
    }
    return 0;
}
```

**파싱 및 코드 생성 순서**:

| 단계 | 파싱 요소 | 액션 | emit되는 코드 |
|------|-----------|------|---------------|
| 1 | `FOR` | for_head 진입 | - |
| 2 | `'('` | - | - |
| 3 | `i = 0` (init) | init 코드 생성 | `push_reg fp` `push_const 1` `add` `push_const 0` `assign` `fetch` |
| 4 | `';'` | init 결과 버림, 라벨 생성, cond 라벨 emit | `shift_sp -1` `Lcond_0:` |
| 5 | `i < 3` (cond) | 조건 코드 생성 | `push_reg fp` `push_const 1` `add` `fetch` `push_const 3` `less` |
| 6 | `';'` | branch_false, jump body, update 라벨 | `branch_false Lend_3` `jump Lbody_2` `Lupdate_1:` |
| 7 | `i = i + 1` (update) | update 코드 생성 | `push_reg fp` `...` `assign` `fetch` |
| 8 | mid-rule | update 결과 버림, jump cond, body 라벨 | `shift_sp -1` `jump Lcond_0` `Lbody_2:` |
| 9 | `')'` | - | - |
| 10 | `{ write_int(i); }` (body) | 본문 코드 생성 | `push_reg fp` `...` `write_int` |
| 11 | stmt 완료 | jump update, end 라벨 | `jump Lupdate_1` `Lend_3:` |

**생성된 IR 코드**:
```asm
; === for (i = 0; i < 3; i = i + 1) ===

; init: i = 0
push_reg fp
push_const 1
add
push_const 0
assign
fetch
shift_sp -1           ; init 결과 버림

; cond label
Lcond_0:

; cond: i < 3
push_reg fp
push_const 1
add
fetch
push_const 3
less
branch_false Lend_3   ; 조건 거짓 → 종료

; 첫 iteration에서 update 건너뛰기
jump Lbody_2

; update label (continue 타겟)
Lupdate_1:

; update: i = i + 1
push_reg fp
push_const 1
add
push_reg fp
push_const 1
add
fetch
push_const 1
add
assign
fetch
shift_sp -1           ; update 결과 버림
jump Lcond_0          ; 조건으로

; body label
Lbody_2:

; body: write_int(i)
push_reg fp
push_const 1
add
fetch
write_int
jump Lupdate_1        ; update로

; end label (break 타겟)
Lend_3:
```

### 6.6 실행 트레이스 (i = 0, 1, 2, 3)

| Iteration | 실행 순서 | i 값 | 출력 |
|-----------|-----------|------|------|
| - | init (i=0) | 0 | - |
| 1 | cond(참) → jump Lbody → body → jump Lupdate | 0 | 0 |
| 2 | update(i=1) → cond(참) → jump Lbody → body → jump Lupdate | 1 | 1 |
| 3 | update(i=2) → cond(참) → jump Lbody → body → jump Lupdate | 2 | 2 |
| 4 | update(i=3) → cond(거짓) → branch_false Lend | 3 | - |

**최종 출력**: `012`

### 6.7 break/continue와의 연동

```
for 문에서:
- break → jump Lend_N (break_label)
- continue → jump Lupdate_N (continue_label) → update → cond → ...
```

**while과의 차이점**:
| 항목 | while | for |
|------|-------|-----|
| continue 타겟 | 조건 검사 (cond) | update 식 |
| 라벨 개수 | 2개 (cond, end) | 4개 (cond, update, body, end) |

---

## 7. break / continue 문

### 7.1 코드 생성 템플릿

```asm
; break
jump <current_loop.break_label>

; continue
jump <current_loop.continue_label>
```

### 7.2 구현 (subc.y)

```yacc
| BREAK ';'
  {
    if (loop_stack_top > 0) {
      emit("jump %s", loop_stack[loop_stack_top-1].break_label);
    }
    REDUCE("stmt->BREAK ';'");
  }
| CONTINUE ';'
  {
    if (loop_stack_top > 0) {
      emit("jump %s", loop_stack[loop_stack_top-1].continue_label);
    }
    REDUCE("stmt->CONTINUE ';'");
  }
```

### 7.3 중첩 루프에서의 동작

```c
for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
        if (j == 1) continue;  // 내부 for의 update로
        if (j == 2) break;     // 내부 for 종료
    }
    if (i == 2) break;         // 외부 for 종료
}
```

- `loop_stack`은 LIFO(스택) 구조로 중첩 루프를 관리
- `break`/`continue`는 항상 가장 안쪽(현재) 루프에 영향

---

## 8. 수정된 파일 및 위치 요약

### subc.y

| 위치 | 내용 |
|------|------|
| line 34-46 | 라벨 스택 및 loop_labels 구조체 정의 |
| line 702-728 | if/else 규칙 (if_head 사용) |
| line 731-738 | while 규칙 (while_head 사용) |
| line 739-755 | for 규칙 (for_head 사용) |
| line 756-771 | break/continue 규칙 |
| line 774-782 | if_head 비단말 정의 |
| line 784-816 | while_head, for_head 비단말 정의 |

---

## 9. 테스트 결과

### Basic Tests
| 테스트 | 결과 |
|--------|------|
| t1.c ~ t4.c | ✅ 모두 통과 |
| input.c | ✅ "hello world" |

### Advanced Tests
| 테스트 | 기대값 | 결과 |
|--------|--------|------|
| if.c | 0 | ✅ |
| func1.c | 20 | ✅ |
| func2.c | 1 | ✅ |
| func3.c | 10 | ✅ |
| struct1~3.c | - | ✅ 모두 통과 |
| INCu.c | 17,17,17,18 | ✅ |

### Custom Tests
| 테스트 | 기대값 | 결과 |
|--------|--------|------|
| for.c | x,y,z 배열 출력 | ✅ |
| while.c | 23 | ✅ |
| test_if1~5 | - | ✅ |
| test_while1~3 | - | ✅ |
| test_for1~3 | - | ✅ |
| test_break/continue | - | ✅ |

---

## 10. 핵심 설계 결정 요약

1. **branch_false의 pop 특성 활용**: 조건값 정리를 위한 추가 코드 불필요

2. **비단말 분리 (if_head, while_head, for_head)**:
   - Mid-rule action을 깔끔하게 분리
   - reduce-reduce 충돌 해결

3. **loop_stack 4필드 구조**:
   - while과 for의 continue 타겟 차이를 일관된 인터페이스로 처리
   - break/continue가 loop_stack만 참조하면 됨

4. **for의 jump Lbody 트릭**:
   - 파싱 순서(update→body)와 실행 순서(body→update)의 불일치 해결
   - 추가 버퍼링 없이 syntax-directed 방식 유지

---

## 11. 참고: expr_e (빈 표현식 처리)

```yacc
expr_e
  : expr
    { $$ = $1; }
  | /* empty */
    { $$.type = NULL; ... }  /* type이 NULL이면 빈 표현식 */
  ;
```

for문에서:
- init이 비어있으면: `shift_sp -1` 생략
- cond가 비어있으면: `push_const 1` emit (무한루프)
- update가 비어있으면: `shift_sp -1` 생략
