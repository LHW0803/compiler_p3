/*
 * File Name    : subc.y
 * Description  : a skeleton bison input
 *
 * Course       : Introduction to Compilers
 * Dept. of Electrical and Computer Engineering, Seoul National University
 */

%{
/* Prologue section */
#include <stdio.h>
#include <stdlib.h>  /* malloc을 위해 필요 */
#include "subc.h"

int   yylex ();
int   yyerror (char* s);
int   get_lineno();
char* yyget_text();
void  reduce(char* s);

/* [Project 3] Add filename variable for error messages */
char* filename;

/* Phase 3 Step D: Function declarations support */
static struct decl *pending_func = NULL;  /* 현재 함수 declaration 전달용 */

/* 헬퍼 함수: pointer 레벨 적용 */
static struct decl *apply_pointers(struct decl *base, int levels) {
    int i;
    struct decl *t = base;
    for (i = 0; i < levels && t != NULL; i++) {
        t = makeptrdecl(t);
    }
    return t;
}
%}

/* Bison declarations section */

/* yylval types */
%union {
  int   intVal;
  char  *stringVal;
  struct id *idptr;      /* Phase 2: ID용 추가 */
  struct decl *declptr;  /* Phase 3 준비용 추가 */
  struct exprinfo expr;  /* Phase 3 Step E: 표현식 정보 */
  struct exprinfo *exprlist;  /* Phase 3 Step F: 인자 리스트용 */
}

/* Tokens and Types */
/* Add more tokens and types */
%token<declptr>   TYPE
%token            STRUCT
%token            SYM_NULL RETURN IF ELSE WHILE FOR BREAK CONTINUE
%token            LOGICAL_OR LOGICAL_AND RELOP EQUOP INCOP DECOP STRUCTOP
%token<idptr>     ID
%token<stringVal> CHAR_CONST STRING
%token<intVal>    INTEGER_CONST

/* Non-terminal types for Phase 3 */
%type<declptr>    type_specifier struct_specifier func_decl
%type<intVal>     pointers
%type<expr>       unary binary expr assignment expr_e
%type<exprlist>   args

/* Precedences and Associativity (low to high) */
%left ','
%right '='
%left LOGICAL_OR
%left LOGICAL_AND
%left EQUOP
%left RELOP
%left '+' '-'
%left '*' '/' '%'
%right '!' UMINUS USTAR UAMP
%left INCOP DECOP '(' ')' '[' ']' '.' STRUCTOP

/* Solution of dangling-else using precedence */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

/* Grammar rules section*/
program
  : ext_def_list
  {
    REDUCE("program->ext_def_list");
  }
  ;

ext_def_list
  : ext_def_list ext_def
  {
    REDUCE("ext_def_list->ext_def_list ext_def");
  }
  | /* empty */
  {
    REDUCE("ext_def_list->epsilon");
  }
  ;

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
  | struct_specifier ';'
  {
    REDUCE("ext_def->struct_specifier ';'");
  }
  | func_decl compound_stmt
  {
    REDUCE("ext_def->func_decl compound_stmt");
  }
  ;

type_specifier
  : TYPE
  {
    /* lexer에서 이미 inttype/chartype/voidtype을 declptr로 실어줌 */
    $$ = $1;
    REDUCE("type_specifier->TYPE");
  }
  | struct_specifier
  {
    $$ = $1;  /* struct_specifier now returns proper declptr */
    REDUCE("type_specifier->struct_specifier");
  }
  ;

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

func_decl
  : type_specifier pointers ID '(' ')'
  {
    pending_func = NULL;  /* 이전 함수 잔여값 클리어 */
    
    if ($1 == NULL) {
      $$ = NULL;  /* 타입 에러 전파 */
    } else {
      /* 반환 타입 생성 */
      struct decl *rettype = apply_pointers($1, $2);
      
      if (rettype == NULL) {
        $$ = NULL;
      } else {
        /* 함수 declaration 생성 */
        struct decl *funcdecl = makefuncdecl();
        
        /* 재선언 검사 */
        if (findcurrentdecl($3) != NULL) {
          error_redeclaration();
          $$ = NULL;
        } else {
          declare($3, funcdecl);
          funcdecl->returntype = rettype;
          funcdecl->formals = NULL;  /* 매개변수 없음 */
          
          /* compound_stmt로 전달 */
          pending_func = funcdecl;
          
          $$ = funcdecl;
        }
      }
    }
    REDUCE("func_decl->type_specifier pointers ID '(' ')'");
  }
  | type_specifier pointers ID '('
  {
    pending_func = NULL;  /* 초기화 */
    
    struct decl *rettype = NULL;
    struct decl *funcdecl = NULL;
    
    if ($1 != NULL) {
      rettype = apply_pointers($1, $2);
    }
    
    /* 함수 선언 자체가 유효한지 체크 */
    if ($1 == NULL || rettype == NULL) {
      /* type 에러 */
      funcdecl = NULL;
    } else if (findcurrentdecl($3) != NULL) {
      error_redeclaration();
      funcdecl = NULL;
    } else {
      funcdecl = makefuncdecl();
      declare($3, funcdecl);
      funcdecl->returntype = rettype;
    }
    
    /* 매개변수 수집용 scope는 항상 연다 (균형 보장) */
    push_scope();
    /* returnid는 여기서 declare하지 않음 - formals 전용 scope */
    
    $<declptr>$ = funcdecl;
  }
  param_list ')'
  {
    struct decl *funcdecl = $<declptr>5;
    struct ste *formals = pop_scope();  /* 실패든 성공이든 항상 pop */
    
    if (funcdecl == NULL) {
      /* 에러 상태: formals 버리고 NULL 전파 */
      pending_func = NULL;
      $$ = NULL;
    } else {
      /* 정상 처리: formals는 헤더 정보로만 보관 */
      funcdecl->formals = formals;  /* 순수 매개변수 리스트 (역순!) */
      pending_func = funcdecl;       /* compound_stmt로 전달 */
      
      $$ = funcdecl;
    }
    REDUCE("func_decl->type_specifier pointers ID '(' param_list ')'");
  }
  ;

pointers
  : '*' pointers
  {
    $$ = $2 + 1;  /* count pointers */
    REDUCE("pointers->'*' pointers");
  }
  | /* empty */
  {
    $$ = 0;  /* no pointer */
    REDUCE("pointers->epsilon");
  }
  ;

param_list
  : param_decl
  {
    REDUCE("param_list->param_decl");
  }
  | param_list ',' param_decl
  {
    REDUCE("param_list->param_list ',' param_decl");
  }
  ;

param_decl
  : type_specifier pointers ID
  {
    if ($1 == NULL) {
      /* type 에러 전파 - declare 하지 않음 */
    } else {
      struct decl *paramtype = apply_pointers($1, $2);
      
      if (paramtype == NULL) {
        /* 포인터 생성 실패 */
      } else {
        struct decl *vardecl = makevardecl(paramtype);
        
        /* 매개변수 중복 검사 */
        if (findcurrentdecl($3) != NULL) {
          error_redeclaration();
        } else {
          declare($3, vardecl);
        }
      }
    }
    REDUCE("param_decl->type_specifier pointers ID");
  }
  | type_specifier pointers ID '[' INTEGER_CONST ']'
  {
    if ($1 == NULL) {
      /* type 에러 전파 */
    } else {
      struct decl *basetype = apply_pointers($1, $2);
      
      if (basetype == NULL) {
        /* 에러 전파 */
      } else {
        /* 배열 매개변수: 배열 타입 그대로 유지 (포인터 decay 없음) */
        struct decl *elemvar = makevardecl(basetype);
        struct decl *arraydecl = makearraydecl($5, elemvar);
        struct decl *paramdecl = makeconstdecl(arraydecl);
        
        if (findcurrentdecl($3) != NULL) {
          error_redeclaration();
        } else {
          declare($3, paramdecl);
        }
      }
    }
    REDUCE("param_decl->type_specifier pointers ID '[' INTEGER_CONST ']'");
  }
  ;

def_list
  : def_list def
  {
    REDUCE("def_list->def_list def");
  }
  | /* empty */
  {
    REDUCE("def_list->epsilon");
  }
  ;

def
  : type_specifier pointers ID ';'
  {
    /* same logic as ext_def */
    if ($1 == NULL) {
      /* type error propagation */
    } else {
      struct decl *basetype = $1;
      int i;  /* C90 compatibility */
      
      for (i = 0; i < $2; i++) {
        basetype = makeptrdecl(basetype);
      }
      
      struct decl *vardecl = makevardecl(basetype);
      
      if (findcurrentdecl($3) != NULL) {
        error_redeclaration();
      } else {
        declare($3, vardecl);
      }
    }
    /* do not use $$ */
    REDUCE("def->type_specifier pointers ID ';'");
  }
  | type_specifier pointers ID '[' INTEGER_CONST ']' ';'
  {
    /* same as ext_def array */
    if ($1 == NULL) {
      /* type error propagation */
    } else {
      struct decl *basetype = $1;
      int i;  /* C90 compatibility */
      
      for (i = 0; i < $2; i++) {
        basetype = makeptrdecl(basetype);
      }
      
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
    REDUCE("def->type_specifier pointers ID '[' INTEGER_CONST ']' ';'");
  }
  ;

compound_stmt
  : '{'
  {
    push_scope();
    
    /* 함수 body인 경우 returnid와 매개변수들을 새로 declare */
    if (pending_func != NULL) {
      /* 1) returnid 설치 */
      declare(returnid, pending_func->returntype);
      
      /* Step G: 현재 함수의 반환 타입 추적 */
      current_func_ret_type = pending_func->returntype;
      
      $<declptr>$ = pending_func;  /* 함수 body 시작을 표시 */
      
      /* 2) formals 리스트 순회하며 매개변수 설치 
       * 주의: formals는 역순 (c→b→a)이므로 
       * 설치 후에는 body scope에서도 역순으로 쌓임 */
      struct ste *p = pending_func->formals;
      while (p != NULL) {
        /* 새로운 ste를 body scope에 생성 */
        declare(p->name, p->decl);
        p = p->prev;
      }
      
      pending_func = NULL;  /* 사용 완료 */
    } else {
      $<declptr>$ = NULL;  /* 중첩된 블록 표시 */
    }
  }
  def_list stmt_list '}'
  {
    pop_scope();
    
    /* Step G: 함수 body 종료 시에만 current_func_ret_type 정리 */
    if ($<declptr>2 != NULL) {
      current_func_ret_type = NULL;
    }
    
    REDUCE("compound_stmt->'{' def_list stmt_list '}'");
  }
  ;

stmt_list
  : stmt_list stmt
  {
    REDUCE("stmt_list->stmt_list stmt");
  }
  | /* empty */
  {
    REDUCE("stmt_list->epsilon");
  }
  ;

stmt
  : expr ';'
  {
    REDUCE("stmt->expr ';'");
  }
  | compound_stmt
  {
    REDUCE("stmt->compound_stmt");
  }
  | RETURN ';'
  {
    /* Step G: current_func_ret_type 사용 */
    if (current_func_ret_type != NULL) {
      if (current_func_ret_type != voidtype) {
        error_return();  /* void가 아닌 함수에서 값 없이 return */
      }
    }
    /* current_func_ret_type == NULL: 함수 밖 return - 조용히 무시 */
    REDUCE("stmt->RETURN ';'");
  }
  | RETURN expr ';'
  {
    /* Step G: current_func_ret_type 사용 */
    /* 에러 전파: $2.type == NULL이면 expr에서 이미 에러 */
    if (current_func_ret_type != NULL && $2.type != NULL) {
      if (current_func_ret_type == voidtype) {
        error_return();  /* void 함수에서 값 반환 */
      } else if (!check_same_type(current_func_ret_type, $2.type)) {
        error_return();  /* 타입 불일치 */
      }
    }
    /* current_func_ret_type == NULL: 함수 밖 return - 조용히 무시 */
    REDUCE("stmt->RETURN expr ';'");
  }
  | ';'
  {
    REDUCE("stmt->';'");
  }
  | IF '(' expr ')' stmt %prec LOWER_THAN_ELSE
  {
    REDUCE("stmt->IF '(' expr ')' stmt");
  }
  | IF '(' expr ')' stmt ELSE stmt
  {
    REDUCE("stmt->IF '(' expr ')' stmt ELSE stmt");
  }
  | WHILE '(' expr ')' stmt
  {
    REDUCE("stmt->WHILE '(' expr ')' stmt");
  }
  | FOR '(' expr_e ';' expr_e ';' expr_e ')' stmt
  {
    REDUCE("stmt->FOR '(' expr_e ';' expr_e ';' expr_e ')' stmt");
  }
  | BREAK ';'
  {
    REDUCE("stmt->BREAK ';'");
  }
  | CONTINUE ';'
  {
    REDUCE("stmt->CONTINUE ';'");
  }
  ;

expr_e
  : expr
  {
    $$ = $1;
    REDUCE("expr_e->expr");
  }
  | /* empty */
  {
    $$.type = NULL;
    $$.is_lvalue = 0;
    $$.is_var = 0;
    $$.is_null_const = 0;
    $$.next = NULL;
    REDUCE("expr_e->epsilon");
  }
  ;

expr
  : assignment
  {
    $$ = $1;
    REDUCE("expr->assignment");
  }
  ;

assignment
  : binary
  {
    $$ = $1;
    REDUCE("assignment->binary");
  }
  | unary '=' assignment
  {
    /* 에러 체크 우선순위:
     * 1. 기존 에러 전파
     * 2. lvalue 검사
     * 3. NULL 리터럴 특수 처리
     * 4. 타입 호환성 검사
     */
    if ($1.type == NULL || ($3.type == NULL && !$3.is_null_const)) {
      /* 1. 기존 에러 전파 */
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!$1.is_lvalue) {
      /* 2. lvalue 검사 */
      error_assignable();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if ($3.is_null_const) {
      /* 3. NULL 리터럴 검사 */
      if (!is_pointer_type($1.type)) {
        error_null();
        $$.type = NULL;
        $$.is_lvalue = 0;
        $$.is_var = 0;
        $$.is_null_const = 0;
      } else {
        $$ = $1;  /* OK: 포인터에 NULL 할당 */
      }
    } else if (!check_same_type($1.type, $3.type)) {
      /* 4. 타입 호환성 검사 */
      error_incompatible();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$ = $1;  /* 할당 성공 */
    }
    REDUCE("assignment->unary '=' assignment");
  }
  ;

binary
  : binary RELOP binary
  {
    if ($1.type == NULL || $3.type == NULL) {
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!check_same_type($1.type, $3.type)) {
      error_comparable();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!is_int_type($1.type) && !is_char_type($1.type)) {
      error_comparable();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$.type = inttype;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    }
    REDUCE("binary->binary RELOP binary");
  }
  | binary EQUOP binary
  {
    if ($1.type == NULL || $3.type == NULL) {
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!check_same_type($1.type, $3.type)) {
      error_comparable();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!is_int_type($1.type) && 
               !is_char_type($1.type) && 
               !is_pointer_type($1.type)) {
      error_comparable();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$.type = inttype;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    }
    REDUCE("binary->binary EQUOP binary");
  }
  | binary '+' binary
  {
    if ($1.type == NULL || $3.type == NULL) {
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!is_int_type($1.type) || !is_int_type($3.type)) {
      error_binary();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$.type = inttype;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    }
    REDUCE("binary->binary '+' binary");
  }
  | binary '-' binary
  {
    if ($1.type == NULL || $3.type == NULL) {
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!is_int_type($1.type) || !is_int_type($3.type)) {
      error_binary();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$.type = inttype;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    }
    REDUCE("binary->binary '-' binary");
  }
  | binary '*' binary
  {
    if ($1.type == NULL || $3.type == NULL) {
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!is_int_type($1.type) || !is_int_type($3.type)) {
      error_binary();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$.type = inttype;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    }
    REDUCE("binary->binary '*' binary");
  }
  | binary '/' binary
  {
    if ($1.type == NULL || $3.type == NULL) {
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!is_int_type($1.type) || !is_int_type($3.type)) {
      error_binary();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$.type = inttype;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    }
    REDUCE("binary->binary '/' binary");
  }
  | binary '%' binary
  {
    if ($1.type == NULL || $3.type == NULL) {
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!is_int_type($1.type) || !is_int_type($3.type)) {
      error_binary();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$.type = inttype;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    }
    REDUCE("binary->binary '%' binary");
  }
  | binary LOGICAL_AND binary
  {
    if ($1.type == NULL || $3.type == NULL) {
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!is_int_type($1.type) || !is_int_type($3.type)) {
      error_binary();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$.type = inttype;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    }
    REDUCE("binary->binary LOGICAL_AND binary");
  }
  | binary LOGICAL_OR binary
  {
    if ($1.type == NULL || $3.type == NULL) {
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!is_int_type($1.type) || !is_int_type($3.type)) {
      error_binary();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$.type = inttype;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    }
    REDUCE("binary->binary LOGICAL_OR binary");
  }
  | unary %prec '='
  {
    $$ = $1;
    REDUCE("binary->unary");
  }
  ;

unary
  : '(' expr ')'
  {
    $$ = $2;  /* 모든 속성 전파 */
    REDUCE("unary->'(' expr ')'");
  }
  | INTEGER_CONST
  {
    $$.type = inttype;
    $$.is_lvalue = 0;
    $$.is_var = 0;
    $$.is_null_const = 0;
    $$.next = NULL;
    REDUCE("unary->INTEGER_CONST");
  }
  | CHAR_CONST
  {
    $$.type = chartype;
    $$.is_lvalue = 0;
    $$.is_var = 0;
    $$.is_null_const = 0;
    $$.next = NULL;
    REDUCE("unary->CHAR_CONST");
  }
  | STRING
  {
    $$.type = makeptrdecl(chartype);  /* char* 타입 생성 */
    $$.is_lvalue = 0;
    $$.is_var = 0;
    $$.is_null_const = 0;
    $$.next = NULL;
    REDUCE("unary->STRING");
  }
  | ID
  {
    struct decl *d = lookup($1);
    if (d == NULL) {
      error_undeclared();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
      $$.next = NULL;
    } else if (d->declclass == DECL_VAR) {
      struct decl *t = get_type(d);
      $$.type = t;  /* Step G fix: use get_type result instead of d->type */
      $$.is_lvalue = 1;
      
      /* & 연산자는 스칼라 변수(int, char)에만 허용 */
      if (is_pointer_type(t) || is_array_type(t) || is_struct_type(t)) {
        $$.is_var = 0;  /* &p, &arr, &s 모두 금지 */
      } else {
        $$.is_var = 1;  /* int, char 등만 허용 */
      }
      
      $$.is_null_const = 0;
      $$.next = NULL;
    } else if (d->declclass == DECL_CONST) {
      $$.type = d->type;
      $$.is_lvalue = 0;   /* 배열은 lvalue 아님 */
      $$.is_var = 0;      /* &arr 불가 */
      $$.is_null_const = 0;
      $$.next = NULL;
    } else if (d->declclass == DECL_FUNC) {
      /* 함수 이름은 함수 decl 자체를 type으로 */
      $$.type = d;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
      $$.next = NULL;
    } else {
      error_undeclared();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
      $$.next = NULL;
    }
    REDUCE("unary->ID");
  }
  | '-' unary %prec '!'
  {
    if ($2.type == NULL) {
      $$ = $2;  /* 에러 전파 */
    } else if (!is_int_or_char_type($2.type)) {
      error_unary();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$.type = $2.type;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    }
    REDUCE("unary->'-' unary");
  }
  | '!' unary
  {
    if ($2.type == NULL) {
      $$ = $2;
    } else if (!is_int_type($2.type)) {
      error_unary();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$.type = inttype;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    }
    REDUCE("unary->'!' unary");
  }
  | unary INCOP
  {
    if ($1.type == NULL) {
      $$ = $1;
    } else if (!$1.is_lvalue) {
      error_assignable();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!is_int_or_char_type($1.type)) {
      error_unary();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$ = $1;  /* 타입 변화 없음 */
    }
    REDUCE("unary->unary INCOP");
  }
  | unary DECOP
  {
    if ($1.type == NULL) {
      $$ = $1;
    } else if (!$1.is_lvalue) {
      error_assignable();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!is_int_or_char_type($1.type)) {
      error_unary();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$ = $1;  /* 타입 변화 없음 */
    }
    REDUCE("unary->unary DECOP");
  }
  | INCOP unary
  {
    if ($2.type == NULL) {
      $$ = $2;
    } else if (!$2.is_lvalue) {
      error_assignable();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!is_int_or_char_type($2.type)) {
      error_unary();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$ = $2;  /* 타입 변화 없음 */
    }
    REDUCE("unary->INCOP unary");
  }
  | DECOP unary
  {
    if ($2.type == NULL) {
      $$ = $2;
    } else if (!$2.is_lvalue) {
      error_assignable();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!is_int_or_char_type($2.type)) {
      error_unary();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$ = $2;  /* 타입 변화 없음 */
    }
    REDUCE("unary->DECOP unary");
  }
  | '&' unary %prec '!'
  {
    if ($2.type == NULL) {
      $$ = $2;
    } else if (!$2.is_var) {
      error_addressof();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$.type = makeptrdecl($2.type);
      $$.is_lvalue = 0;  /* &a는 rvalue */
      $$.is_var = 0;
      $$.is_null_const = 0;
    }
    REDUCE("unary->'&' unary");
  }
  | '*' unary %prec '!'
  {
    if ($2.type == NULL) {
      $$ = $2;
    } else if (!is_pointer_type($2.type)) {
      error_indirection();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      struct decl *pointee = get_type($2.type)->ptrto;
      $$.type = pointee;
      $$.is_lvalue = 1;  /* *p는 lvalue */
      $$.is_var = 1;     /* &(*p)도 가능 */
      $$.is_null_const = 0;
    }
    REDUCE("unary->'*' unary");
  }
  | unary '[' expr ']'
  {
    if ($1.type == NULL || $3.type == NULL) {
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!is_array_type($1.type)) {
      error_array();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!is_int_type($3.type)) {
      error_subscript();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      /* 배열의 element 타입 가져오기 */
      struct decl *elem = get_type($1.type)->elementvar;
      $$.type = (elem && elem->type) ? elem->type : NULL;
      $$.is_lvalue = 1;  /* arr[i]는 lvalue */
      $$.is_var = 1;     /* &arr[i] 가능 */
      $$.is_null_const = 0;
    }
    REDUCE("unary->unary '[' expr ']'");
  }
  | unary '.' ID
  {
    if ($1.type == NULL) {
      $$ = $1;
    } else if (!is_struct_type($1.type)) {
      error_struct();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      /* struct member 찾기 */
      struct ste *field = get_type($1.type)->fieldlist;
      struct decl *member_decl = NULL;
      while (field != NULL) {
        if (field->name == $3) {
          member_decl = field->decl;
          break;
        }
        field = field->prev;
      }
      if (member_decl == NULL) {
        error_member();
        $$.type = NULL;
        $$.is_lvalue = 0;
        $$.is_var = 0;
        $$.is_null_const = 0;
      } else {
        $$.type = member_decl->type;  /* VAR의 type */
        $$.is_lvalue = $1.is_lvalue;  /* struct가 lvalue면 member도 lvalue */
        $$.is_var = $1.is_lvalue;      /* lvalue member는 &가능 */
        $$.is_null_const = 0;
      }
    }
    REDUCE("unary->unary '.' ID");
  }
  | unary STRUCTOP ID
  {
    if ($1.type == NULL) {
      $$ = $1;
    } else if (!is_pointer_type($1.type)) {
      error_strurctp();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      struct decl *pointee = get_type($1.type)->ptrto;
      if (!is_struct_type(pointee)) {
        error_strurctp();
        $$.type = NULL;
        $$.is_lvalue = 0;
        $$.is_var = 0;
        $$.is_null_const = 0;
      } else {
        /* struct member 찾기 */
        struct ste *field = get_type(pointee)->fieldlist;
        struct decl *member_decl = NULL;
        while (field != NULL) {
          if (field->name == $3) {
            member_decl = field->decl;
            break;
          }
          field = field->prev;
        }
        if (member_decl == NULL) {
          error_member();
          $$.type = NULL;
          $$.is_lvalue = 0;
          $$.is_var = 0;
          $$.is_null_const = 0;
        } else {
          $$.type = member_decl->type;  /* VAR의 type */
          $$.is_lvalue = 1;  /* p->member는 항상 lvalue */
          $$.is_var = 1;      /* &(p->member) 가능 */
          $$.is_null_const = 0;
        }
      }
    }
    REDUCE("unary->unary STRUCTOP ID");
  }
  | unary '(' args ')'
  {
    /* 인자 리스트에 에러가 있는지 먼저 확인 */
    struct exprinfo *it = $3;
    int has_arg_error = 0;
    while (it != NULL) {
        if (it->type == NULL) {
            has_arg_error = 1;
            break;
        }
        it = it->next;
    }
    
    if (has_arg_error) {
        /* 인자에서 이미 에러 발생 - 전파만 */
        $$.type = NULL;
        $$.is_lvalue = 0;
        $$.is_var = 0;
        $$.is_null_const = 0;
        $$.next = NULL;
    } else if ($1.type == NULL) {
        /* 함수명 자체에서 에러 - 전파만 */
        $$.type = NULL;
        $$.is_lvalue = 0;
        $$.is_var = 0;
        $$.is_null_const = 0;
        $$.next = NULL;
    } else {
        struct decl *callee = $1.type;
        
        if (callee == NULL || callee->declclass != DECL_FUNC) {
            error_function();   /* not a function */
            $$.type = NULL;
            $$.is_lvalue = 0;
            $$.is_var = 0;
            $$.is_null_const = 0;
            $$.next = NULL;
        } else if (!check_function_arguments(callee, $3)) {
            error_arguments();  /* incompatible arguments */
            $$.type = NULL;
            $$.is_lvalue = 0;
            $$.is_var = 0;
            $$.is_null_const = 0;
            $$.next = NULL;
        } else {
            /* 호출 성공: 반환 타입 전파 */
            $$.type = callee->returntype;
            $$.is_lvalue = 0;
            $$.is_var = 0;
            $$.is_null_const = 0;
            $$.next = NULL;
        }
    }
    REDUCE("unary->unary '(' args ')'");
  }
  | unary '(' ')'
  {
    if ($1.type == NULL) {
        $$.type = NULL;
        $$.is_lvalue = 0;
        $$.is_var = 0;
        $$.is_null_const = 0;
        $$.next = NULL;
    } else {
        struct decl *callee = $1.type;
        
        if (callee == NULL || callee->declclass != DECL_FUNC) {
            error_function();
            $$.type = NULL;
            $$.is_lvalue = 0;
            $$.is_var = 0;
            $$.is_null_const = 0;
            $$.next = NULL;
        } else if (callee->formals != NULL) {
            /* formals가 있는데 인자가 없음 */
            error_arguments();
            $$.type = NULL;
            $$.is_lvalue = 0;
            $$.is_var = 0;
            $$.is_null_const = 0;
            $$.next = NULL;
        } else {
            /* 인자 없음, formals도 없음 -> OK */
            $$.type = callee->returntype;
            $$.is_lvalue = 0;
            $$.is_var = 0;
            $$.is_null_const = 0;
            $$.next = NULL;
        }
    }
    REDUCE("unary->unary '(' ')'");
  }
  | SYM_NULL
  {
    $$.type = inttype;  /* NULL은 일단 int로 취급 */
    $$.is_lvalue = 0;
    $$.is_var = 0;
    $$.is_null_const = 1;  /* NULL 리터럴 표시 */
    $$.next = NULL;
    REDUCE("unary->SYM_NULL");
  }
  ;

args
  : expr
  {
      /* 첫 번째 인자: 새 노드를 만들어서 head로 사용 */
      struct exprinfo *node = (struct exprinfo *)malloc(sizeof(struct exprinfo));
      *node = $1;
      node->next = NULL;
      
      $$ = node;
      REDUCE("args->expr");
  }
  | args ',' expr
  {
      /* 추가 인자: 새 노드를 만들어 앞에 붙이기 (역순 리스트) */
      struct exprinfo *node = (struct exprinfo *)malloc(sizeof(struct exprinfo));
      *node = $3;
      node->next = $1;   /* 기존 리스트 앞에 새 노드 */
      $$ = node;
      
      REDUCE("args->args ',' expr");
  }
  ;

%%

/* Epilogue section */
/* add return 0 */
int yyerror (char* s) {
  // fprintf (stderr, "yyerror: %s at line %d, token '%s'\n", s, get_lineno(), yyget_text());
  return 0;
}

/* sometime, it prints only empty lines */
void reduce(char* s) {
  printf("%s\n", s);
}

/* ========================================================================== */
/* [Project 3] Error message printing procedures                              */
/* ========================================================================== */

// Print the preamble of error message.
void error_preamble(void) {
  printf("%s:%d: error: ", filename, get_lineno());
}

void error_undeclared(void) {
  error_preamble();
  printf("use of undeclared identifier\n");
}

void error_redeclaration(void) {
  error_preamble();
  printf("redeclaration\n");
}

void error_assignable(void) {
  error_preamble();
  printf("lvalue is not assignable\n");
}

void error_incompatible(void) {
  error_preamble();
  printf("incompatible types for assignment operation\n");
}

void error_null(void) {
  error_preamble();
  printf("cannot assign 'NULL' to non-pointer type\n");
}

void error_binary(void) {
  error_preamble();
  printf("invalid operands to binary expression\n");
}

void error_unary(void) {
  error_preamble();
  printf("invalid argument type to unary expression\n");
}

void error_comparable(void) {
  error_preamble();
  printf("types are not comparable in binary expression\n");
}

void error_indirection(void) {
  error_preamble();
  printf("indirection requires pointer operand\n");
}

void error_addressof(void) {
  error_preamble();
  printf("cannot take the address of an rvalue\n");
}

void error_struct(void) {
  error_preamble();
  printf("member reference base type is not a struct\n");
}

void error_strurctp(void){
  error_preamble();
  printf("member reference base type is not a struct pointer\n");
}

void error_member(void) {
  error_preamble();
  printf("no such member in struct\n");
}

void error_array(void) {
  error_preamble();
  printf("subscripted value is not an array\n");
}

void error_subscript(void) {
  error_preamble();
  printf("array subscript is not an integer\n");
}

void error_incomplete(void) {
  error_preamble();
  printf("incomplete type\n");
}

void error_return(void) {
  error_preamble();
  printf("incompatible return types\n");
}

/* error_function and error_arguments are defined in symtab.c */
