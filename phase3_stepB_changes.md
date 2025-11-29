# Phase 3 Step B: Variable Declarations 구현 변경사항

## 개요
Phase 3 Step B에서는 변수 선언에 대한 semantic action을 구현했습니다. 이제 변수와 배열이 symbol table에 등록되고, 포인터와 배열 타입이 올바르게 처리됩니다.

## 주요 변경 파일

### subc.y (Parser)

#### 1. %type 선언 추가 (line 44)
```yacc
/* Non-terminal types for Phase 3 */
%type<declptr>    type_id type_specifier
%type<intVal>     pointers  /* 추가: 포인터 개수 관리 */
```

#### 2. pointers 규칙 수정 (line 158-169)
```yacc
pointers
  : '*' pointers
  {
    $$ = $2 + 1;  /* count pointers */
    REDUCE("pointers->'*' pointers");
  }
  | %empty
  {
    $$ = 0;  /* no pointer */
    REDUCE("pointers->epsilon");
  }
  ;
```

**변경사항**:
- 기존: 단일 포인터만 지원 (`'*' | %empty`)
- 변경: 다중 포인터 지원 (`'*' pointers`로 재귀)
- 포인터 개수를 int로 관리 (더 안전하고 확장 가능)

#### 3. ext_def 규칙 - 변수 선언 (line 83-108)
```yacc
ext_def
  : type_specifier pointers ID ';'
  {
    if ($1 == NULL) {
      /* type error propagation from Step A */
    } else {
      struct decl *basetype = $1;
      int i;  /* C90 compatibility */
      
      /* apply pointer layers */
      for (i = 0; i < $2; i++) {
        basetype = makeptrdecl(basetype);
      }
      
      struct decl *vardecl = makevardecl(basetype);
      
      /* redeclaration check */
      if (findcurrentdecl($3) != NULL) {
        error_redeclaration();
      } else {
        declare($3, vardecl);
      }
    }
    /* do not use $$ */
    REDUCE("ext_def->type_specifier pointers ID ';'");
  }
```

#### 4. ext_def 규칙 - 배열 선언 (line 109-135)
```yacc
  | type_specifier pointers ID '[' INTEGER_CONST ']' ';'
  {
    if ($1 == NULL) {
      /* type error propagation */
    } else {
      struct decl *basetype = $1;
      int i;  /* C90 compatibility */
      
      /* apply pointer layers */
      for (i = 0; i < $2; i++) {
        basetype = makeptrdecl(basetype);
      }
      
      /* array declaration: CONST(ARRAY(VAR)) */
      struct decl *elemvar = makevardecl(basetype);
      struct decl *arraydecl = makearraydecl($5, elemvar);
      struct decl *constdecl = makeconstdecl(arraydecl);
      
      if (findcurrentdecl($3) != NULL) {
        error_redeclaration();
      } else {
        declare($3, constdecl);
      }
    }
    /* do not use $$ */
    REDUCE("ext_def->type_specifier pointers ID '[' INTEGER_CONST ']' ';'");
  }
```

**핵심 패턴**:
- CONST → ARRAY → VAR 구조 (reference 준수)
- 배열은 lvalue가 아니므로 CONST로 감싸기
- elementvar는 VAR declaration을 가리킴

#### 5. def 규칙 - 로컬 변수/배열 (line 248-298)
```yacc
def
  : type_specifier pointers ID ';'
  {
    /* same logic as ext_def */
    // 동일한 코드...
  }
  | type_specifier pointers ID '[' INTEGER_CONST ']' ';'
  {
    /* same as ext_def array */
    // 동일한 코드...
  }
```

**설계 결정**: ext_def와 완전히 동일한 로직
- scope 차이는 symtab이 자동으로 처리
- 코드 중복이지만 명확하고 안전

#### 6. compound_stmt 수정 - Scope 처리 (line 301-310)
```yacc
compound_stmt
  : '{'
  {
    push_scope();  /* enter new scope */
  }
  def_list stmt_list '}'
  {
    pop_scope();  /* exit scope */
    REDUCE("compound_stmt->'{' def_list stmt_list '}'");
  }
  ;
```

**중요**: 미드룰 액션 사용하여 '{'에서 즉시 scope push

## 설계 특징

### 1. 포인터 개수 관리
```c
int *p;      // pointers = 1 → makeptrdecl 1번
int **p;     // pointers = 2 → makeptrdecl 2번
int ***p;    // pointers = 3 → makeptrdecl 3번
```

### 2. 타입 구축 과정
```
type_specifier (예: int) 
→ pointers 적용 (예: int*) 
→ makevardecl (변수) 또는 makearraydecl (배열)
→ declare로 symbol table 등록
```

### 3. 에러 처리
- Type 에러 전파: `if ($1 == NULL)` 체크
- Redeclaration: `findcurrentdecl()` 사용
- 에러 발생 시에도 파싱 계속 진행

### 4. C90 호환성
```c
int i;  /* 변수 선언을 블록 시작에 */
for (i = 0; i < $2; i++) { ... }
```

## 테스트 결과

### 성공 케이스
```c
int x;              // 기본 변수 ✓
int *p;             // 포인터 ✓
int **pp;           // 이중 포인터 ✓
int arr[10];        // 배열 ✓
int *ptrs[5];       // 포인터 배열 ✓
void *generic;      // void 포인터 ✓

int main() {
    int local;      // 로컬 변수 ✓
    int arr[20];    // 로컬 배열 ✓
}
```

### 에러 검출
```c
int x;
int x;              // error: redeclaration ✓

foo bar;            // error: use of undeclared identifier ✓
                    // (Step A에서 처리)
```

## Reference와의 차이점 및 개선사항

### 1. Grammar 구조 차이
- Reference: `var_list` 방식 (여러 변수 한번에)
- 우리: 개별 선언 방식 (더 단순하고 명확)

### 2. 포인터 처리 개선
- Reference: pointer를 직접 TYPE으로 생성
- 우리: 포인터 개수만 관리 → 더 안전하고 확장 가능

### 3. Scope 처리
- compound_stmt에서 미드룰 액션으로 즉시 scope 전환
- 함수 블록과 일반 블록 모두 일관되게 처리

## 다음 단계 준비
Step B 완료로 이제 변수 선언이 symbol table에 등록되므로:
- Step C: struct 타입과 멤버 처리
- Step D: 함수 선언과 매개변수 처리
- Step E 이후: 선언된 변수들의 타입 검사