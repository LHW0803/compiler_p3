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

/* pop_scope: scope 종료 시 호출 */
void pop_scope() {
    def_stack_top = scope_markers[--scope_level];
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