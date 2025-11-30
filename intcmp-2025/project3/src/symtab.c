/*
 * File Name    : symtab.c
 * Description  : Symbol table implementation for semantic analyzer
 *
 * Course       : Introduction to Compilers
 * Dept. of Electrical and Computer Engineering, Seoul National University
 */

#include "subc.h"
#include "subc.tab.h"
#include <stdlib.h>
#include <string.h>

/* Definition stack - reference style implementation */
static struct ste *def_stack_top = NULL;
static struct ste *scope_markers[100];
static int scope_level = 0;

/* push_scope: 새 scope 진입 시 호출 */
void push_scope() {
    scope_markers[scope_level++] = def_stack_top;
}

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

/* lookup: 현재 scope부터 상위로 검색 */
struct decl* lookup(struct id *name) {
    for (struct ste *e = def_stack_top; e != NULL; e = e->prev) {
        if (e->name == name) {
            return e->decl;
        }
    }
    return NULL;
}

/* declare: 현재 scope에 새 선언 추가 */
struct ste* declare(struct id *name, struct decl *decl) {
    struct ste *new_ste = (struct ste*)malloc(sizeof(struct ste));
    new_ste->name = name;
    new_ste->decl = decl;
    new_ste->prev = def_stack_top;
    def_stack_top = new_ste;
    return new_ste;
}

/* findcurrentdecl: 현재 scope에서만 검색 (redeclaration check용) */
struct decl* findcurrentdecl(struct id *name) {
    struct ste *bound = (scope_level > 0) ? scope_markers[scope_level-1] : NULL;
    for (struct ste *e = def_stack_top; e != bound; e = e->prev) {
        if (e->name == name) {
            return e->decl;
        }
    }
    return NULL;
}

/* check_is_type: TYPE declaration인지 확인 */
int check_is_type(struct decl *decl) {
    if (decl == NULL) {
        return 0;
    }
    return (decl->declclass == DECL_TYPE);
}

/* 전역 변수 - built-in types */
struct decl *inttype = NULL;
struct decl *chartype = NULL; 
struct decl *voidtype = NULL;
struct id *returnid = NULL;
struct decl *current_func_ret_type = NULL;

/* Helper 함수들 */

/* maketypedecl: TYPE declaration 생성 */
struct decl* maketypedecl(int typeclass) {
    struct decl *d = (struct decl*)malloc(sizeof(struct decl));
    memset(d, 0, sizeof(struct decl));  /* 모든 필드 0으로 초기화 */
    d->declclass = DECL_TYPE;
    d->typeclass = typeclass;
    
    /* 기본 타입의 크기 설정 */
    switch(typeclass) {
        case TYPE_INT:
        case TYPE_CHAR:
            d->size = 4;  /* 4 bytes */
            break;
        case TYPE_VOID:
            d->size = 0;
            break;
        default:
            d->size = 0;
    }
    return d;
}

/* makevardecl: VAR declaration 생성 */
struct decl* makevardecl(struct decl *type) {
    struct decl *d = (struct decl*)malloc(sizeof(struct decl));
    memset(d, 0, sizeof(struct decl));
    d->declclass = DECL_VAR;
    d->type = type;
    d->size = (type != NULL) ? type->size : 0;
    return d;
}

/* makeptrdecl: pointer TYPE declaration 생성 */
struct decl* makeptrdecl(struct decl *type) {
    struct decl *d = (struct decl*)malloc(sizeof(struct decl));
    memset(d, 0, sizeof(struct decl));
    d->declclass = DECL_TYPE;
    d->typeclass = TYPE_PTR;
    d->ptrto = type;
    d->size = 4;  /* pointer는 4 bytes */
    return d;
}

/* makearraydecl: array TYPE declaration 생성 */
struct decl* makearraydecl(int size, struct decl *elementvar) {
    struct decl *d = (struct decl*)malloc(sizeof(struct decl));
    memset(d, 0, sizeof(struct decl));
    d->declclass = DECL_TYPE;
    d->typeclass = TYPE_ARRAY;
    d->elementvar = elementvar;
    d->num_index = size;
    
    /* array 크기 계산 */
    if (elementvar && elementvar->type) {
        d->size = size * elementvar->type->size;
    } else {
        d->size = 0;
    }
    return d;
}

/* makeconstdecl: CONST declaration 생성 */
struct decl* makeconstdecl(struct decl *type) {
    struct decl *d = (struct decl*)malloc(sizeof(struct decl));
    memset(d, 0, sizeof(struct decl));
    d->declclass = DECL_CONST;
    d->type = type;
    return d;
}

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

/* makefuncdecl: FUNC declaration 생성 */
struct decl* makefuncdecl(void) {
    struct decl *d = (struct decl*)malloc(sizeof(struct decl));
    memset(d, 0, sizeof(struct decl));
    
    d->declclass = DECL_FUNC;
    /* formals, returntype는 나중에 설정 */
    /* 다른 필드들은 0/NULL로 초기화됨 */
    
    return d;
}

/* get_type: 항상 TYPE decl을 반환하도록 정규화 */
struct decl* get_type(struct decl *d) {
    if (d == NULL) return NULL;
    if (d->declclass == DECL_VAR || d->declclass == DECL_CONST)
        return d->type;
    return d;  /* 이미 TYPE */
}

/* Type checking helper functions */
int is_int_type(struct decl *t) {
    t = get_type(t);
    return (t && t->declclass == DECL_TYPE && t->typeclass == TYPE_INT);
}

int is_char_type(struct decl *t) {
    t = get_type(t);
    return (t && t->declclass == DECL_TYPE && t->typeclass == TYPE_CHAR);
}

int is_int_or_char_type(struct decl *t) {
    return is_int_type(t) || is_char_type(t);
}

int is_pointer_type(struct decl *t) {
    t = get_type(t);
    return (t && t->declclass == DECL_TYPE && t->typeclass == TYPE_PTR);
}

int is_array_type(struct decl *t) {
    t = get_type(t);
    return (t && t->declclass == DECL_TYPE && t->typeclass == TYPE_ARRAY);
}

int is_struct_type(struct decl *t) {
    t = get_type(t);
    return (t && t->declclass == DECL_TYPE && t->typeclass == TYPE_STRUCT);
}

/* check_same_type: 타입 동일성 검사 */
int check_same_type(struct decl *t1, struct decl *t2) {
    t1 = get_type(t1);
    t2 = get_type(t2);
    
    if (t1 == NULL || t2 == NULL) return 0;
    if (t1 == t2) return 1;  /* 같은 타입 decl */
    
    if (t1->typeclass != t2->typeclass) return 0;
    
    /* 포인터는 가리키는 타입까지 재귀 비교 */
    if (t1->typeclass == TYPE_PTR) {
        return check_same_type(t1->ptrto, t2->ptrto);
    }
    
    return 0;
}

/* init_type: built-in 타입 초기화 */
void init_type() {
    /* built-in 타입 생성 */
    inttype = maketypedecl(TYPE_INT);
    chartype = maketypedecl(TYPE_CHAR);
    voidtype = maketypedecl(TYPE_VOID);
    
    /* global scope에 등록 */
    declare(enter(ID, "int", 3), inttype);
    declare(enter(ID, "char", 4), chartype);
    declare(enter(ID, "void", 4), voidtype);
    
    /* return용 특수 ID */
    returnid = enter(ID, "*return", 7);
}

/* Error function implementations for Step F */
void error_function(void) {
    error_preamble();
    printf("not a function\n");
}

void error_arguments(void) {
    error_preamble();
    printf("incompatible arguments in function call\n");
}

/* Check function arguments match formals */
int check_function_arguments(struct decl *funcdecl, struct exprinfo *actuals) {
    struct ste *formal = funcdecl->formals;
    struct exprinfo *actual = actuals;
    
    /* formals와 actuals 비교 - 둘 다 역순이므로 순서대로 매칭됨 */
    while (formal != NULL && actual != NULL) {
        /* formal->decl은 DECL_VAR이고, formal->decl->type이 실제 타입 */
        struct decl *formal_type = get_type(formal->decl);
        /* actual->type은 이미 정규화된 TYPE이어야 함 */
        if (!check_same_type(formal_type, actual->type)) {
            return 0;  /* 타입 불일치 */
        }
        formal = formal->prev;
        actual = actual->next;
    }
    
    /* 개수 확인 */
    if (formal != NULL || actual != NULL) {
        return 0;  /* 인자 개수 다름 */
    }
    
    return 1;  /* 인자 일치 */
}