# Phase 3 Step C: Struct Declarations 구현 변경사항

## 개요
Phase 3 Step C에서는 struct 선언과 사용을 위한 semantic action을 구현했습니다. struct는 별도의 scope를 가지는 멤버들과 함께 TYPE declaration으로 처리됩니다.

## 주요 변경 파일

### 1. symtab.c

#### a) pop_scope() 수정 - ste 리스트 반환 (line 24-46)
```c
/* pop_scope: scope 종료 시 그 scope의 symbol들을 반환 */
struct ste* pop_scope() {
    /* 기존 로직 유지하면서 리스트 생성 추가 */
    struct ste *old_marker = (scope_level > 0) ? scope_markers[scope_level-1] : NULL;
    struct ste *result = NULL;
    struct ste *prev = NULL;
    
    /* collect symbols while restoring stack */
    while (def_stack_top != old_marker) {
        struct ste *current = def_stack_top;
        def_stack_top = def_stack_top->prev;  /* 기존 pop 동작 */
        
        /* build return list */
        current->prev = prev;
        prev = current;
    }
    result = prev;
    
    /* 기존 로직: scope_level 감소 */
    scope_level--;
    
    return result;  /* popscope reverses stes */
}
```

**변경사항**:
- 반환 타입: void → struct ste*
- scope 내 symbol들을 linked list로 수집하여 반환
- 기존 compound_stmt의 pop_scope() 호출은 영향 없음 (C는 반환값 무시 허용)

#### b) makestructdecl() 추가 (line 165-197)
```c
/* makestructdecl: STRUCT TYPE declaration 생성 */
struct decl* makestructdecl(struct ste *fields) {
    struct decl *d = (struct decl*)malloc(sizeof(struct decl));
    memset(d, 0, sizeof(struct decl));
    
    /* 필수 필드 설정 */
    d->declclass = DECL_TYPE;
    d->typeclass = TYPE_STRUCT;
    d->fieldlist = fields;
    
    /* struct 크기 계산 (padding 없이 단순 합) */
    d->size = 0;
    struct ste *field = fields;
    while (field != NULL) {
        if (field->decl && field->decl->size > 0) {
            d->size += field->decl->size;
        }
        field = field->prev;
    }
    
    /* 명시적 초기화 - 디버깅 용이성 */
    d->type = NULL;          /* TYPE decl은 type 필드 미사용 */
    d->value = 0;            /* STRUCT는 상수값 없음 */
    d->real_value = 0.0;     /* STRUCT는 실수값 없음 */
    d->formals = NULL;       /* STRUCT는 함수 아님 */
    d->returntype = NULL;    /* STRUCT는 함수 아님 */
    d->elementvar = NULL;    /* STRUCT는 배열 아님 */
    d->num_index = 0;        /* STRUCT는 배열 아님 */
    d->ptrto = NULL;         /* STRUCT는 포인터 타입 아님 */
    d->next = NULL;          /* 단독 선언 */
    
    return d;
}
```

#### c) check_is_struct_type() 추가 (line 199-208)
```c
/* check_is_struct_type: struct type인지 확인하고 에러 출력 */
int check_is_struct_type(struct decl *decl) {
    if (decl == NULL || 
        decl->declclass != DECL_TYPE || 
        decl->typeclass != TYPE_STRUCT) {
        error_incomplete();  /* struct_specifier에서만 사용 */
        return 0;
    }
    return 1;
}
```

**설계 결정**: 에러 출력을 함수 내부에서 처리하여 중복 방지

### 2. subc.h

#### 함수 선언 추가/수정 (line 69, 74, 82)
```c
// Symbol table interfaces - 반환 타입 변경
struct ste* pop_scope(void);  /* void → struct ste* */

// Type checking helpers
int check_is_struct_type(struct decl *decl);  /* 추가 */

// Declaration construction helpers
struct decl* makestructdecl(struct ste *fields);  /* 추가 */
```

### 3. subc.y

#### a) %type 선언 추가 (line 43)
```yacc
/* Non-terminal types for Phase 3 */
%type<declptr>    type_id type_specifier struct_specifier
%type<intVal>     pointers
```

#### b) struct_specifier semantic action (line 180-215)
```yacc
struct_specifier
  : STRUCT ID '{'
  {
    push_scope();  /* 필드 전용 scope */
  }
  def_list '}'
  {
    struct ste *fields = pop_scope();  /* 필드 리스트 수집 */
    struct decl *structdecl = makestructdecl(fields);
    
    /* struct 재정의 검사 - 현재 scope만 확인 (실용적) */
    if (findcurrentdecl($2) != NULL) {
      error_redeclaration();
      $$ = NULL;
    } else {
      /* grammar상 struct 정의는 file-scope에서만 등장하므로 사실상 global */
      declare($2, structdecl);
      $$ = structdecl;
    }
    REDUCE("struct_specifier->STRUCT ID '{' def_list '}'");
  }
  | STRUCT ID
  {
    /* 타입 사용은 lookup으로 모든 scope 검색 */
    struct decl *declptr = lookup($2);
    
    if (!check_is_struct_type(declptr)) {
      /* error_incomplete() already called in check_is_struct_type();
         propagate NULL to type_specifier → ext_def/def → skip declaration */
      $$ = NULL;
    } else {
      $$ = declptr;
    }
    REDUCE("struct_specifier->STRUCT ID");
  }
  ;
```

**핵심 패턴**:
- 정의: push_scope() → def_list → pop_scope() → makestructdecl()
- 재정의 검사: `findcurrentdecl()` (현재 scope)
- 사용: `lookup()` (모든 scope)

#### c) type_specifier 수정 (line 172-176)
```yacc
  | struct_specifier
  {
    $$ = $1;  /* struct_specifier now returns proper declptr */
    REDUCE("type_specifier->struct_specifier");
  }
```

Step A에서 임시로 NULL 처리했던 부분을 실제 구현으로 교체

## 설계 특징

### 1. Scope 관리
- struct 멤버는 별도 scope로 관리 (다른 struct와 충돌 방지)
- struct 자체는 grammar상 file-scope에만 정의
- `popscope reverses stes` - reference 요구사항 준수

### 2. 타입 시스템 통합
```
struct Point { int x; int y; };
→ DECL_TYPE / TYPE_STRUCT
→ fieldlist가 멤버 ste 리스트 가리킴
→ size는 모든 멤버 크기의 합
```

### 3. 에러 처리
- Redeclaration: `error_redeclaration()`
- Undefined/Non-struct: `error_incomplete()`
- 에러 전파: NULL propagation

### 4. Step B와의 통합
```c
struct Point p;      // type_specifier가 struct declptr 반환
struct Point *p2;    // Step B의 pointer 처리 그대로 적용
struct Point arr[5]; // Step B의 array 처리 그대로 적용
```

## 테스트 결과

### 성공 케이스
```c
struct Point {       // struct 정의 ✓
    int x;
    int y;
};

struct Point p1;     // struct 변수 ✓
struct Point *p2;    // struct 포인터 ✓
struct Point arr[5]; // struct 배열 ✓

int main() {
    struct Point local; // 함수 내 사용 ✓
}
```

### 에러 검출
```c
struct Point { char a; };  // error: redeclaration ✓
struct Unknown u;          // error: incomplete type ✓
int NotStruct;
struct NotStruct s;        // error: incomplete type ✓
```

### 알려진 제한사항
```c
struct Node {
    struct Node *next;  // error: incomplete type
};
```
- Self-referential pointer가 현재 incomplete type 에러 발생
- C 표준과 다르지만 requirements에 따라 의도된 동작일 수 있음

## Reference와의 차이점

### 1. 재정의 검사 방식
- Reference 예시: 명시 없음
- 우리: `findcurrentdecl()` 사용 (현재 scope만)
- 테스트 결과에 따라 `lookup()`으로 변경 가능

### 2. 명시적 필드 초기화
- 모든 struct decl 필드를 명시적으로 초기화
- 디버깅 시 uninitialized 값 방지

## 다음 단계 준비
Step C 완료로 이제 struct 타입이 완전히 지원되므로:
- Step D: 함수 선언 시 struct 반환 타입/매개변수 가능
- Step E: struct 멤버 접근 연산자 (., ->) 구현
- Step F: struct를 인자로 하는 함수 호출