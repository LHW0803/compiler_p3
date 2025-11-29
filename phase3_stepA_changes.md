# Phase 3 Step A: Type Recognition Layer 구현 변경사항

## 개요
Phase 3 Step A에서는 TYPE 토큰 없이 ID를 통해 타입을 인식하는 semantic layer를 구현했습니다. 이는 symbol table lookup을 통해 ID가 실제 타입인지 검증하는 첫 번째 semantic check입니다.

## 주요 변경 파일

### 1. subc.y (Parser)

#### a) %type 선언 추가 (line 42-43)
```yacc
/* Non-terminal types for Phase 3 */
%type<declptr>    type_id type_specifier
```
- `type_id`와 `type_specifier`가 이제 `struct decl*` 타입을 반환합니다.

#### b) type_id semantic action 구현 (line 101-119)
```yacc
type_id
  : ID
  {
    /* lookup and check if it's a type */
    struct decl *declptr = lookup($1);
    
    if (declptr == NULL) {
      error_undeclared();
      $$ = NULL;
    } else if (!check_is_type(declptr)) {
      /* not a type declaration */
      error_undeclared();  /* TODO: may need different error later */
      $$ = NULL;
    } else {
      $$ = declptr;
    }
    REDUCE("type_id->ID");
  }
  ;
```

**핵심 로직:**
- `lookup($1)`: 모든 scope를 검색하여 ID 찾기 (built-in 타입 포함)
- `check_is_type()`: 찾은 declaration이 TYPE인지 확인
- 에러 시 `error_undeclared()` 호출하고 NULL 반환 (에러 전파)

#### c) type_specifier 수정 (line 121-133)
```yacc
type_specifier
  : type_id
  {
    $$ = $1;  /* propagate type_id result */
    REDUCE("type_specifier->type_id");
  }
  | struct_specifier
  {
    /* struct_specifier not yet implemented - temporary NULL */
    $$ = NULL;  /* TODO: implement in Step C */
    REDUCE("type_specifier->struct_specifier");
  }
  ;
```
- `type_id`의 결과를 상위로 전달
- struct는 Step C에서 구현 예정

### 2. 변경 없음 (이미 Phase 1/2에서 구현)
- **symtab.c**: `lookup()`, `check_is_type()` 등 이미 구현됨
- **subc.h**: 필요한 구조체와 함수 선언 이미 존재

## 테스트 결과

### 1. 선언되지 않은 타입 사용
```c
foo x;
```
**출력:** `test_error.c:2: error: use of undeclared identifier`

### 2. 변수를 타입으로 사용
```c
int y;
y z;
```
**출력:** `test_var_as_type.c:2: error: use of undeclared identifier`

### 3. 올바른 타입 사용
```c
int x;    // OK
char y;   // OK
```
**출력:** 에러 없음

## 설계 결정사항

### 1. `lookup()` vs `findcurrentdecl()`
- **선택**: `lookup()` 사용
- **이유**: built-in 타입(int, char, void)은 global scope에 있으므로 모든 scope에서 접근 가능해야 함

### 2. 에러 메시지 전략
- **현재**: 모든 경우 `error_undeclared()` 사용
- **TODO**: requirements 분석 후 필요시 세분화
  - `declptr == NULL` → `error_undeclared()`
  - `declptr->declclass != DECL_TYPE` → 별도 에러 함수

### 3. struct_specifier 처리
- **현재**: `$$ = NULL` 임시 처리
- **계획**: Step C에서 struct declaration 구현 시 수정

## Reference 스타일과의 일관성
- `check_is_type()` 함수 활용으로 추상화 레벨 유지
- 향후 에러 처리 로직 변경 시 한 곳만 수정하면 됨

## 다음 단계 준비
Step A 완료로 이제 `type_specifier`가 `struct decl*`을 반환하므로:
- Step B: 변수 선언 시 타입 정보 활용 가능
- Step C: struct 타입 처리 추가
- Step D 이후: 타입 검사 본격 구현

## 보고서용 / reference ppt와의 차이점
lookup vs findcurrentdecl – 이번 Step A에서의 개선점

레퍼런스 코드 일부는 type 인식에 findcurrentdecl 같은 걸 쓰기도 하는데,
우리는 의도적으로 lookup을 써서 개선했습니다.

findcurrentdecl

“현재 scope에서만” 찾습니다.

그러면 함수 안에서 int x; 선언할 때,
로컬 scope에는 아직 int가 없으니 “undeclared type” 에러가 나버리는 문제가 생길 수 있어요.

lookup

전역 → 로컬 순으로 chain을 타고 올라가며 찾습니다.

init_type이 전역에서 int, char, void를 선언해두기 때문에
어디서든 타입으로 쓸 수 있음.

그래서 지금 구조는:

“타입 이름은 항상 전역이나 상위 scope에서 유효”
→ 타입을 찾을 때는 무조건 lookup,
→ “같은 scope 안에서의 redeclaration” 같은 건 findcurrentdecl 로 검사

라는 역할 분리를 잘 지키고 있습니다.