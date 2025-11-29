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

// Hash table interfaces
unsigned hash(char *name);
id *enter(int tokenType, char *name, int length);

// Symbol table interfaces
void push_scope(void);
void pop_scope(void);
struct decl* lookup(struct id *name);
struct ste* declare(struct id *name, struct decl *decl);
struct decl* findcurrentdecl(struct id *name);
int check_is_type(struct decl *decl);

// Declaration construction helpers
struct decl* maketypedecl(int typeclass);
struct decl* makevardecl(struct decl *type);
struct decl* makeptrdecl(struct decl *type);
struct decl* makearraydecl(int size, struct decl *elementvar);
struct decl* makeconstdecl(struct decl *type);
void init_type(void);

// Global built-in types
extern struct decl *inttype;
extern struct decl *chartype;
extern struct decl *voidtype;
extern struct id *returnid;

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
