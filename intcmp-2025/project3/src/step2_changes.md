# Step 2: symtab.c 생성 및 구현

## 변경된/생성된 파일
- `symtab.c` (새로 생성)
- `subc.h` (함수 선언 추가)
- `Makefile` (symtab.o 추가)

## symtab.c 구현 내용

### 1. Definition Stack 관리
```c
static struct ste *def_stack_top = NULL;
static struct ste *scope_markers[100];
static int scope_level = 0;
```

### 2. Scope 관리 함수
- `push_scope()`: 새 scope 진입
- `pop_scope()`: scope 종료

### 3. Symbol Table 검색 함수
- `lookup()`: 모든 scope에서 검색
- `findcurrentdecl()`: 현재 scope에서만 검색

### 4. Symbol Table 추가 함수
- `declare()`: 현재 scope에 새 선언 추가

### 5. Helper 함수들
- `maketypedecl()`: TYPE declaration 생성
- `makevardecl()`: VAR declaration 생성
- `makeptrdecl()`: pointer TYPE 생성
- `makearraydecl()`: array TYPE 생성
- `makeconstdecl()`: CONST declaration 생성
- `check_is_type()`: TYPE declaration 확인

### 6. 초기화 함수
- `init_type()`: built-in 타입(int, char, void) 등록

### 7. 전역 변수
```c
struct decl *inttype = NULL;
struct decl *chartype = NULL; 
struct decl *voidtype = NULL;
struct id *returnid = NULL;
```

## subc.h 추가 내용

### 함수 프로토타입 선언 (line 67-87)
- Symbol table 함수들
- Declaration 생성 helper 함수들  
- 전역 타입 변수 extern 선언

## Makefile 수정
- OBJECTS에 `symtab.o` 추가

## 컴파일 테스트
- Docker 환경에서 성공적으로 컴파일됨
- 에러 없음

## 특이사항
- symtab.c에 `#include "subc.tab.h"` 필요 (ID 토큰 사용)

## 다음 단계
Step 3: subc.y의 main()에서 init_type() 호출