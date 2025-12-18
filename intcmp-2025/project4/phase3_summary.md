# Phase 3 구현 요약

## 개요

**목표**: 포인터와 배열 코드 생성
**테스트 파일**: t1.c (포인터), 자체 테스트 (배열)
**구현 순서**: Step 3-1 → Step 3-2

---

## Step 3-1: 포인터 기본 연산

### 목표
포인터 역참조(`*p`), 주소 연산(`&x`), NULL 리터럴 코드 생성

### 구현 사항

**1. 포인터 역참조 (`*E`) - subc.y:1239~1269**

```c
| '*' unary %prec '!'
{
  /* [Project 4] 포인터 역참조
   * - operand가 l-value면 fetch로 포인터 값 획득
   * - operand가 r-value면 이미 포인터 값이므로 그대로
   * - 결과는 항상 l-value (포인터 값이 새 주소가 됨)
   * - semantic error 경로에서는 emit 하지 않음
   */
  if ($2.type == NULL) {
    $$ = $2;
  } else if (!is_pointer_type($2.type)) {
    error_indirection();
    $.type = NULL;
    ...
  } else {
    /* 정상 경로: 코드 생성 */
    if ($2.is_lvalue) {
      emit("fetch");  /* 포인터 값 획득 */
    }
    /* 결과: 스택에 포인터 값(=주소)이 있고, 이것이 *E의 l-value */

    struct decl *pointee = get_type($2.type)->ptrto;
    $.type = pointee;
    $.is_lvalue = 1;  /* *p는 lvalue */
    $.is_var = 1;     /* &(*p)도 가능 */
    ...
  }
}
```

### 핵심 개념: `*E`는 L-value 생성

| 상황 | operand | 코드 생성 | 결과 |
|------|---------|-----------|------|
| `*p` | p는 VAR, l-value | `fetch` 발행 | 포인터 값이 l-value |
| `**pp` | *pp는 r-value | fetch 없음 | 포인터 값이 l-value |

### 스택 트레이스 (`*p = 3`)

```
단계  명령어              스택 상태         설명
───────────────────────────────────────────────────────────
 1   push_const Lglob+0  [addr_p]         p의 주소 (l-value)
 2   fetch               [ptr_val]        p의 값 = x의 주소 (*E 처리)
    (mid-rule 복제)
 3   push_reg sp         [ptr_val, &top]
 4   fetch               [ptr_val, ptr_val]  주소 복제
 5   push_const 3        [ptr_val, ptr_val, 3]
 6   assign              [ptr_val]        x = 3
 7   fetch               [3]              대입 결과값
───────────────────────────────────────────────────────────
최종: 스택에 3, 메모리 x에도 3
```

**2. 주소 연산 (`&x`) - subc.y:1221~1238**

```c
| '&' unary %prec '!'
{
  if ($2.type == NULL) {
    $$ = $2;
  } else if (!$2.is_var) {
    error_addressof();
    ...
  } else {
    /* 코드 생성 없음: operand가 이미 주소를 스택에 가지고 있음 */
    $.type = makeptrdecl($2.type);
    $.is_lvalue = 0;  /* &a는 rvalue */
    ...
  }
}
```

### 핵심: `&x`는 코드 생성 불필요

- `x`를 파싱하면 이미 주소가 스택에 push됨 (l-value)
- `&`는 단순히 **타입만 변환** (int → int*)
- 추가 emit 없음

**3. NULL 리터럴 - subc.y:1504~1515**

```c
| SYM_NULL
{
  /* [Project 4] NULL = 0 */
  emit("push_const 0");

  $.type = inttype;
  $.is_lvalue = 0;
  $.is_var = 0;
  $.is_null_const = 1;  /* semantic에서 포인터 대입 허용 */
  ...
}
```

### 핵심: NULL은 0

- `push_const 0` 발행
- `is_null_const` 플래그로 포인터 대입 시 타입 체크 우회

---

## Step 3-2: 배열 접근

### 목표
배열 인덱싱(`arr[i]`) 코드 생성

### 구현 사항 - subc.y:1270~1330

```c
| unary '[' expr ']'
{
  /* [Project 4] 배열 접근 코드 생성
   * Stack before: [base_addr, index_value_or_addr]
   * 계산: base + index * element_size
   * Stack after: [element_addr] (l-value)
   */
  if (/* 에러 케이스들 */) {
    ...
  } else {
    struct decl *elem = get_type($1.type)->elementvar;

    /* elem == NULL 방어 */
    if (!elem) {
      $.type = NULL;
      ...
    } else {
      int elem_size = size_in_words(elem);

      /* index가 l-value면 fetch하여 값으로 변환 */
      if ($3.is_lvalue) {
        emit("fetch");
      }

      /* element size가 1이 아니면 곱셈 */
      if (elem_size != 1) {
        emit("push_const %d", elem_size);
        emit("mul");
      }

      /* base + offset = element 주소 */
      emit("add");

      /* 결과는 l-value */
      $.type = elem->type;
      $.is_lvalue = 1;  /* arr[i]는 lvalue */
      $.is_var = 1;     /* &arr[i] 가능 */
      ...
    }
  }
}
```

### 핵심 공식

```
element_addr = base_addr + index * element_size_in_words
```

### 스택 트레이스 (`arr[i] = x`)

초기: `arr`이 전역 배열 `Lglob+0`, `i`가 지역 변수 `fp+1`, `x`가 `fp+2`

```
단계  명령어               스택 상태              설명
────────────────────────────────────────────────────────────────
 1   push_const Lglob+0   [base]                arr 배열 base 주소
 2   push_reg fp          [base, fp]            i의 주소 시작
 3   push_const 1         [base, fp, 1]
 4   add                  [base, addr_i]        i의 주소
 5   fetch                [base, idx]           i의 값 (l-value → r-value)
    (elem_size==1이므로 mul 생략)
 6   add                  [elem_addr]           arr[i]의 주소
    (mid-rule 복제)
 7   push_reg sp          [elem_addr, &top]
 8   fetch                [elem_addr, elem_addr]
    (RHS: x 읽기)
 9   push_reg fp          [elem_addr, elem_addr, fp]
10   push_const 2         [elem_addr, elem_addr, fp, 2]
11   add                  [elem_addr, elem_addr, addr_x]
12   fetch                [elem_addr, elem_addr, val_x]  (x l-value fetch)
13   assign               [elem_addr]           arr[i] = val_x
14   fetch                [val_x]               대입 결과값
────────────────────────────────────────────────────────────────
최종: arr[i]에 x 값 저장됨
```

### elem_size가 1보다 큰 경우

```c
int arr[10];  // elem_size = 1 (int = 4 bytes = 1 word)
struct S { int a; int b; } arr2[5];  // elem_size = 2 words
```

`arr2[i]` 접근 시:
```asm
<index fetch if l-value>
push_const 2    ; elem_size
mul             ; index * 2
add             ; base + index * 2
```

---

## 테스트 결과

| 파일 | 내용 | 결과 |
|------|------|------|
| t1.c | `*p = 3` (포인터 역참조 대입) | ✓ pass |
| ptr_test.c | `p = &x`, `*p = 3`, `p = NULL` | ✓ pass |
| arr_test1.c | `arr[0] = 3` (상수 인덱스, LHS) | ✓ pass |
| arr_test2.c | `x = arr[1]` (상수 인덱스, RHS) | ✓ pass |
| arr_test3.c | `arr[i] = x` (변수 인덱스) | ✓ pass |

---

## 핵심 학습 포인트

1. **`*E`는 L-value 생성**: fetch가 아니라 포인터 값을 새 주소로 사용
2. **`&E`는 코드 생성 불필요**: operand가 이미 주소를 push함
3. **배열 접근 공식**: `base + index * elem_size`
4. **index l-value 처리**: 변수 인덱스인 경우 fetch 필요
5. **semantic error guard**: 에러 경로에서는 emit 하지 않음

---

## 관련 코드 위치

| 기능 | 파일:라인 |
|------|-----------|
| `*E` 포인터 역참조 | subc.y:1239~1269 |
| `&E` 주소 연산 | subc.y:1221~1238 |
| `SYM_NULL` | subc.y:1504~1515 |
| `E[i]` 배열 접근 | subc.y:1270~1330 |
