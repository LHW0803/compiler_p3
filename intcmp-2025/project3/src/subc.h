/*
 * File Name    : subc.h
 * Description  : A header file for the subc program.
 *
 * Course       : Introduction to Compilers
 * Dept. of Electrical and Computer Engineering, Seoul National University
 */

#ifndef __SUBC_H__
#define __SUBC_H__

#include <stdio.h>
#include <strings.h>

typedef struct id {
  int tokenType;
  char *name;
  int count;
} id;

/* Forward declarations - 상호 참조를 위해 필요 */
struct decl;
struct ste;

/* Symbol table entry */
struct ste {
    struct id *name;
    struct decl *decl;
    struct ste *prev;  /* for definition stack */
};

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

/* Expression info for type propagation */
struct exprinfo {
    struct decl *type;      /* 실제 값의 타입 (항상 TYPE decl, 에러시 NULL) */
    int is_lvalue;          /* = 왼쪽에 올 수 있는가? */
    int is_var;             /* & 연산자 적용 가능한 단순 변수인가? */
    int is_null_const;      /* SYM_NULL 리터럴인가? */
    struct exprinfo *next;  /* 인자 리스트용 */
};

// Hash table interfaces
unsigned hash(char *name);
id *enter(int tokenType, char *name, int length);

// Symbol table interfaces
void push_scope(void);
struct ste* pop_scope(void);  /* void → struct ste* */
struct decl* lookup(struct id *name);
struct ste* declare(struct id *name, struct decl *decl);
struct decl* findcurrentdecl(struct id *name);
int check_is_type(struct decl *decl);
int check_is_struct_type(struct decl *decl);  /* 추가 */

// Declaration construction helpers
struct decl* maketypedecl(int typeclass);
struct decl* makevardecl(struct decl *type);
struct decl* makeptrdecl(struct decl *type);
struct decl* makearraydecl(int size, struct decl *elementvar);
struct decl* makeconstdecl(struct decl *type);
struct decl* makestructdecl(struct ste *fields);  /* 추가 */
struct decl* makefuncdecl(void);  /* Step D 추가 */
void init_type(void);

// Type checking helpers (Step E)
struct decl* get_type(struct decl *d);
int is_int_type(struct decl *t);
int is_char_type(struct decl *t);
int is_int_or_char_type(struct decl *t);
int is_pointer_type(struct decl *t);
int is_array_type(struct decl *t);
int is_struct_type(struct decl *t);
int check_same_type(struct decl *t1, struct decl *t2);
int check_function_arguments(struct decl *funcdecl, struct exprinfo *actuals);

// Global built-in types
extern struct decl *inttype;
extern struct decl *chartype;
extern struct decl *voidtype;
extern struct id *returnid;

// Current function's return type (Step G)
extern struct decl *current_func_ret_type;

// [Project 2] Reduce macro for debugging parse rules
#define REDUCE(s) reduce(s)

// Error message printing procedures
extern char *filename;  /* filename for error messages */
void error_preamble(void);
void error_undeclared(void);
void error_redeclaration(void);
void error_assignable(void);
void error_incompatible(void);
void error_null(void);
void error_binary(void);
void error_unary(void);
void error_comparable(void);
void error_indirection(void);
void error_addressof(void);
void error_struct(void);
void error_strurctp(void);
void error_member(void);
void error_array(void);
void error_subscript(void);
void error_incomplete(void);
void error_return(void);
void error_function(void);
void error_arguments(void);

#endif
