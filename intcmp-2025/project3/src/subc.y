/*
 * File Name    : subc.y
 * Description  : a skeleton bison input
 *
 * Course       : Introduction to Compilers
 * Dept. of Electrical and Computer Engineering, Seoul National University
 */

%{
/* Prologue section */
#include "subc.h"

int   yylex ();
int   yyerror (char* s);
int   get_lineno();
char* yyget_text();
void  reduce(char* s);

/* [Project 3] Add filename variable for error messages */
char* filename;
%}

/* Bison declarations section */

/* yylval types */
%union {
  int   intVal;
  char  *stringVal;
  struct id *idptr;      /* Phase 2: ID용 추가 */
  struct decl *declptr;  /* Phase 3 준비용 추가 */
}

/* Tokens and Types */
/* Add more tokens and types */
%token            STRUCT
%token            SYM_NULL RETURN IF ELSE WHILE FOR BREAK CONTINUE
%token            LOGICAL_OR LOGICAL_AND RELOP EQUOP INCOP DECOP STRUCTOP
%token<idptr>     ID
%token<stringVal> CHAR_CONST STRING
%token<intVal>    INTEGER_CONST

/* Non-terminal types for Phase 3 */
%type<declptr>    type_id type_specifier struct_specifier
%type<intVal>     pointers

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
  | %empty
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

type_id
  : ID
  {
    /* lookup and check if it's a type */
    struct decl *declptr = lookup($1);
    
    if (declptr == NULL) {
      error_undeclared();
      $$ = NULL;
    } else if (!check_is_type(declptr)) {
      /* not a type declaration */
      error_undeclared();  /* TODO: may need different error later */
      $$ = NULL;
    } else {
      $$ = declptr;
    }
    REDUCE("type_id->ID");
  }
  ;

type_specifier
  : type_id
  {
    $$ = $1;  /* propagate type_id result */
    REDUCE("type_specifier->type_id");
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
    REDUCE("func_decl->type_specifier pointers ID '(' ')'");
  }
  | type_specifier pointers ID '(' param_list ')'
  {
    REDUCE("func_decl->type_specifier pointers ID '(' param_list ')'");
  }
  ;

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
    REDUCE("param_decl->type_specifier pointers ID");
  }
  | type_specifier pointers ID '[' INTEGER_CONST ']'
  {
    REDUCE("param_decl->type_specifier pointers ID '[' INTEGER_CONST ']'");
  }
  ;

def_list
  : def_list def
  {
    REDUCE("def_list->def_list def");
  }
  | %empty
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
    push_scope();  /* enter new scope */
  }
  def_list stmt_list '}'
  {
    pop_scope();  /* exit scope */
    REDUCE("compound_stmt->'{' def_list stmt_list '}'");
  }
  ;

stmt_list
  : stmt_list stmt
  {
    REDUCE("stmt_list->stmt_list stmt");
  }
  | %empty
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
  | RETURN expr ';'
  {
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
    REDUCE("expr_e->expr");
  }
  | %empty
  {
    REDUCE("expr_e->epsilon");
  }
  ;

expr
  : unary '=' expr
  {
    REDUCE("expr->unary '=' expr");
  }
  | binary
  {
    REDUCE("expr->binary");
  }
  ;

binary
  : binary RELOP binary
  {
    REDUCE("binary->binary RELOP binary");
  }
  | binary EQUOP binary
  {
    REDUCE("binary->binary EQUOP binary");
  }
  | binary '+' binary
  {
    REDUCE("binary->binary '+' binary");
  }
  | binary '-' binary
  {
    REDUCE("binary->binary '-' binary");
  }
  | binary '*' binary
  {
    REDUCE("binary->binary '*' binary");
  }
  | binary '/' binary
  {
    REDUCE("binary->binary '/' binary");
  }
  | binary '%' binary
  {
    REDUCE("binary->binary '%' binary");
  }
  | binary LOGICAL_AND binary
  {
    REDUCE("binary->binary LOGICAL_AND binary");
  }
  | binary LOGICAL_OR binary
  {
    REDUCE("binary->binary LOGICAL_OR binary");
  }
  | unary %prec '='
  {
    REDUCE("binary->unary");
  }
  ;

unary
  : '(' expr ')'
  {
    REDUCE("unary->'(' expr ')'");
  }
  | INTEGER_CONST
  {
    REDUCE("unary->INTEGER_CONST");
  }
  | CHAR_CONST
  {
    REDUCE("unary->CHAR_CONST");
  }
  | STRING
  {
    REDUCE("unary->STRING");
  }
  | ID
  {
    REDUCE("unary->ID");
  }
  | '-' unary %prec '!'
  {
    REDUCE("unary->'-' unary");
  }
  | '!' unary
  {
    REDUCE("unary->'!' unary");
  }
  | unary INCOP
  {
    REDUCE("unary->unary INCOP");
  }
  | unary DECOP
  {
    REDUCE("unary->unary DECOP");
  }
  | INCOP unary
  {
    REDUCE("unary->INCOP unary");
  }
  | DECOP unary
  {
    REDUCE("unary->DECOP unary");
  }
  | '&' unary %prec '!'
  {
    REDUCE("unary->'&' unary");
  }
  | '*' unary %prec '!'
  {
    REDUCE("unary->'*' unary");
  }
  | unary '[' expr ']'
  {
    REDUCE("unary->unary '[' expr ']'");
  }
  | unary '.' ID
  {
    REDUCE("unary->unary '.' ID");
  }
  | unary STRUCTOP ID
  {
    REDUCE("unary->unary STRUCTOP ID");
  }
  | unary '(' args ')'
  {
    REDUCE("unary->unary '(' args ')'");
  }
  | unary '(' ')'
  {
    REDUCE("unary->unary '(' ')'");
  }
  | SYM_NULL
  {
    REDUCE("unary->SYM_NULL");
  }
  ;

args
  : expr
  {
    REDUCE("args->expr");
  }
  | args ',' expr
  {
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

void error_function(void) {
  error_preamble();
  printf("not a function\n");
}

void error_arguments(void) {
  error_preamble();
  printf("incompatible arguments in function call\n");
}
