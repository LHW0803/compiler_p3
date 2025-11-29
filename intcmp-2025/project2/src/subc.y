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
%}

/* Bison declarations section */

/* yylval types */
%union {
  int   intVal;
  char  *stringVal;
}

/* Tokens and Types */
/* Add more tokens and types */
%token            TYPE STRUCT
%token            SYM_NULL RETURN IF ELSE WHILE FOR BREAK CONTINUE
%token            LOGICAL_OR LOGICAL_AND RELOP EQUOP INCOP DECOP STRUCTOP
%token<stringVal> ID CHAR_CONST STRING
%token<intVal>    INTEGER_CONST

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
    REDUCE("ext_def->type_specifier pointers ID ';'");
  }
  | type_specifier pointers ID '[' INTEGER_CONST ']' ';'
  {
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
    REDUCE("type_specifier->TYPE");
  }
  | struct_specifier
  {
    REDUCE("type_specifier->struct_specifier");
  }
  ;

struct_specifier
  : STRUCT ID '{' def_list '}'
  {
    REDUCE("struct_specifier->STRUCT ID '{' def_list '}'");
  }
  | STRUCT ID
  {
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
  : '*'
  {
    REDUCE("pointers->'*'");
  }
  | %empty
  {
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
    REDUCE("def->type_specifier pointers ID ';'");
  }
  | type_specifier pointers ID '[' INTEGER_CONST ']' ';'
  {
    REDUCE("def->type_specifier pointers ID '[' INTEGER_CONST ']' ';'");
  }
  ;

compound_stmt
  : '{' def_list stmt_list '}'
  {
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