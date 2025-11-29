# Step 1: Symbol Table 구조체 추가

## 변경된 파일
- `subc.h`

## 변경 내용

### subc.h에 추가된 내용

1. **Forward declarations** (line 21-23)
```c
/* Forward declarations - 상호 참조를 위해 필요 */
struct decl;
struct ste;
```

2. **Symbol table entry 구조체** (line 25-30)
```c
/* Symbol table entry */
struct ste {
    struct id *name;
    struct decl *decl;
    struct ste *prev;  /* for definition stack */
};
```

3. **Declaration 구조체** (line 32-47)
```c
/* Declaration 구조체 */
struct decl {
    int declclass;          /* DECL class: VAR, CONST, FUNC, TYPE */
    struct decl *type;      /* VAR, CONST: pointer to its TYPE decl */
    int value;              /* CONST: value of integer constant */
    float real_value;       /* CONST: value of float constant */
    struct ste *formals;    /* FUNC: pointer to formal argument list */
    struct decl *returntype;/* FUNC: pointer to return TYPE decl */
    int typeclass;          /* TYPE: type class: INT, array, ptr, … */
    struct decl *elementvar;/* TYPE (array): ptr to element VAR decl */
    int num_index;          /* TYPE (array): number of elements */
    struct ste *fieldlist;  /* TYPE (struct): ptr to field list */
    struct decl *ptrto;     /* TYPE (pointer): type of the pointer */
    int size;               /* ALL: size in bytes */
    struct decl *next;      /* For list_of_variables declarations */
};
```

4. **상수 정의** (line 49-61)
```c
/* Declaration class 상수 */
#define DECL_VAR    1
#define DECL_CONST  2
#define DECL_FUNC   3
#define DECL_TYPE   4

/* Type class 상수 */
#define TYPE_INT    1
#define TYPE_CHAR   2
#define TYPE_VOID   3
#define TYPE_ARRAY  4
#define TYPE_PTR    5
#define TYPE_STRUCT 6
```

## 컴파일 테스트
- Docker 환경에서 성공적으로 컴파일됨
- 에러 없음

## 다음 단계
Step 2: symtab.c 파일 생성 및 symbol table 함수 구현