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

/* Forward declarations - for cross-reference */
struct decl;
struct ste;

/* Symbol table entry */
struct ste {
    struct id *name;
    struct decl *decl;
    struct ste *prev;  /* for definition stack */
};

/* Declaration struct */
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
    int offset;             /* VAR/CONST: offset (global: Lglob+offset, local: fp+offset) */
    int is_global;          /* VAR/CONST: 1=global, 0=local */
    struct decl *next;      /* For list_of_variables declarations */
};

/* Declaration class const */
#define DECL_VAR    1
#define DECL_CONST  2
#define DECL_FUNC   3
#define DECL_TYPE   4

/* Type class const */
#define TYPE_INT    1
#define TYPE_CHAR   2
#define TYPE_VOID   3
#define TYPE_ARRAY  4
#define TYPE_PTR    5
#define TYPE_STRUCT 6

/* [Project 4] Relational/Equality operator codes */
#define OP_LT  1   /* <  */
#define OP_LE  2   /* <= */
#define OP_GT  3   /* >  */
#define OP_GE  4   /* >= */
#define OP_EQ  5   /* == */
#define OP_NE  6   /* != */

/* Expression info for type propagation */
struct exprinfo {
    struct decl *type;      /* real type (always TYPE decl, NULL if error) */
    int is_lvalue;          /* can be a l value? */
    int is_var;             /* can using & op? */
    int is_null_const;      /* SYM_NULL literal? */
    struct exprinfo *next;  /* for the list */
};

// Hash table interfaces
unsigned hash(char *name);
id *enter(int tokenType, char *name, int length);

// Symbol table interfaces
void push_scope(void);
struct ste* pop_scope(void);  /* void → struct ste* */
struct decl* lookup(struct id *name);
struct ste* declare(struct id *name, struct decl *decl);
struct ste* declare_struct(struct id *name, struct decl *decl);  /* struct -> parents scope entry */
struct decl* findcurrentdecl(struct id *name);
struct decl* findstructdecl(struct id *name);  /* struct -> golabl search */
int check_is_type(struct decl *decl);
int check_is_struct_type(struct decl *decl);

// Declaration construction helpers
struct decl* maketypedecl(int typeclass);
struct decl* makevardecl(struct decl *type);
struct decl* makeptrdecl(struct decl *type);
struct decl* makearraydecl(int size, struct decl *elementvar);
struct decl* makeconstdecl(struct decl *type);
struct decl* makestructdecl(struct ste *fields);
struct decl* makefuncdecl(void);
void init_type(void);
int size_in_words(struct decl *d);  /* decl의 크기를 word 단위로 반환 */

// Type checking helpers
struct decl* get_type(struct decl *d);
int is_int_type(struct decl *t);
int is_char_type(struct decl *t);
int is_int_or_char_type(struct decl *t);
int is_pointer_type(struct decl *t);
int is_array_type(struct decl *t);
int is_struct_type(struct decl *t);
int check_same_type(struct decl *t1, struct decl *t2);
int check_function_arguments(struct decl *funcdecl, struct exprinfo *actuals);

// Global built in types
extern struct decl *inttype;
extern struct decl *chartype;
extern struct decl *voidtype;
extern struct id *returnid;

// Current function return type
extern struct decl *current_func_ret_type;

// [Project 2] Reduce macro for debugging parse rules
// #define REDUCE(s) reduce(s)  // Disabled for final output
#define REDUCE(s) /* do nothing */

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

// === Code Generation (Project 4) ===
extern FILE *outfile;
extern int global_words;           // 전역 변수 총 크기 (word 단위)
extern int emitted_any_function;   // 함수 생성 여부 플래그

void emit(const char *fmt, ...);
void emit_label(const char *label);
char* new_label(const char *prefix);

// === String Literal Table ===
#define MAX_STRINGS 256

char* add_string(const char *str);   // 테이블에 추가, label 포인터 반환
void emit_string_table(void);         // 모든 문자열 출력

// 디버깅 시 emit 후 flush (필요 시 컴파일 옵션으로 활성화: -DEMIT_FLUSH)
// #define EMIT_FLUSH

#endif
