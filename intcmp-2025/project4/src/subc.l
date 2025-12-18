/*                                                                          */
/* File Name    : subc.l                                                    */
/* Description  : a skeleton flex input                                     */
/*                                                                          */
/* Course       : Introduction to Compilers                                 */
/* Dept. of Electrical and Computer Engineering, Seoul National University  */
/*                                                                          */

%{
/* C Code Section */
/* Any code placed inside the brackets will be literally copied into the generated C file. */

#include "subc.h"
#include "subc.tab.h"
#include <stdio.h>    /* fprintf, snprintf */
#include <stdlib.h>   /* exit(), malloc() */
#include <stdarg.h>   /* va_list for emit() */
#include <string.h>   /* strlen, memcpy, strcmp */

int yyparse(void);
static int comment_depth = 0;
static int lineno = 1;

/* === Code Generation (Project 4) === */
FILE *outfile = NULL;
static int label_counter = 0;
int global_words = 0;
int emitted_any_function = 0;

void emit(const char *fmt, ...) {
    if (!outfile) {
        fprintf(stderr, "emit(): outfile is NULL\n");
        exit(1);
    }
    va_list args;
    va_start(args, fmt);
    vfprintf(outfile, fmt, args);
    fputc('\n', outfile);
    va_end(args);
#ifdef EMIT_FLUSH
    fflush(outfile);
#endif
}

void emit_label(const char *label) {
    if (!outfile) {
        fprintf(stderr, "emit_label(): outfile is NULL\n");
        exit(1);
    }
    /* 규약: label 문자열에 ':'를 포함하지 않는다 */
    fprintf(outfile, "%s:\n", label);
#ifdef EMIT_FLUSH
    fflush(outfile);
#endif
}

char* new_label(const char *prefix) {
    /* 주의: malloc 반환 -> 호출자가 free 책임 (또는 프로그램 종료까지 유지) */
    char *buf = (char*)malloc(64);
    if (!buf) {
        fprintf(stderr, "new_label(): malloc failed\n");
        exit(1);
    }
    snprintf(buf, 64, "%s%d", prefix, label_counter++);
    return buf;
}

/* === String Literal Table === */
static char *string_table[MAX_STRINGS];   /* 문자열 (따옴표 포함) */
static char *string_labels[MAX_STRINGS];  /* 라벨: Str0, Str1, ... */
static int string_count = 0;

/* strdup 대체 (POSIX 의존 제거) */
static char* xstrdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = (char*)malloc(n);
    if (!p) {
        fprintf(stderr, "xstrdup(): malloc failed\n");
        exit(1);
    }
    memcpy(p, s, n);
    return p;
}

char* add_string(const char *str) {
    int i;

    /* 중복 체크 (dedup) */
    for (i = 0; i < string_count; i++) {
        if (strcmp(string_table[i], str) == 0) {
            return string_labels[i];  /* 기존 라벨 반환 */
        }
    }

    /* 새 문자열 추가 */
    if (string_count >= MAX_STRINGS) {
        fprintf(stderr, "add_string(): too many strings (%d)\n", string_count);
        exit(1);
    }

    /* 문자열 저장 */
    string_table[string_count] = xstrdup(str);

    /* 라벨 생성 및 저장 */
    char label_buf[16];
    snprintf(label_buf, 16, "Str%d", string_count);
    string_labels[string_count] = xstrdup(label_buf);

    string_count++;
    return string_labels[string_count - 1];
}

void emit_string_table(void) {
    int i;
    for (i = 0; i < string_count; i++) {
        emit("%s. string %s", string_labels[i], string_table[i]);
    }
}
%}

/* Definitions Section */
/* This section contains simple name definitions and declarations of start conditions. */

letter        [A-Za-z_]
digit         [0-9]
id            {letter}({letter}|{digit})*
integer_const ([1-9]{digit}*)|"0"
whitespace    [ \t]
special_char  \\[nt]

%x COMMENT

%%

  /* Rules Section */
  /* This section contains series of rules for recognizing tokens. */
  /* All comments in this section must be indented. */

  /* Keywords */
  /* Add more Keyworks */
"int"                           {
                                  yylval.declptr = inttype;
                                  return TYPE;
                                }
"char"                          {
                                  yylval.declptr = chartype;
                                  return TYPE;
                                }
"void"                          {
                                  yylval.declptr = voidtype;
                                  return TYPE;
                                }
"struct"                        { return STRUCT; }
"NULL"                          { return SYM_NULL; }
"return"                        { return RETURN; }
"if"                            { return IF; }
"else"                          { return ELSE; }
"while"                         { return WHILE; }
"for"                           { return FOR; }
"break"                         { return BREAK; }
"continue"                      { return CONTINUE; }

  /* Identifiers */
{id}                            {
                                  struct id *entered  = enter(ID, yytext, yyleng);
                                  yylval.idptr = entered;
                                  return ID;
                                }

  /* Integer constants */
{integer_const}                 {
                                  yylval.intVal = atoi(yytext);
                                  return INTEGER_CONST;
                                }

  /* String variables */
\"({special_char}|[^\\\"\n])*\" {
                                  yylval.stringVal = yytext;
                                  return STRING;
                                }

  /* Character variables */
'({special_char}|[^\\'\n])'     {
                                  yylval.stringVal = yytext;
                                  return CHAR_CONST;
                                }

  /* Double operators first */
"||"                            { return LOGICAL_OR; }
"&&"                            { return LOGICAL_AND; }
"=="                            { yylval.intVal = OP_EQ; return EQUOP; }
"!="                            { yylval.intVal = OP_NE; return EQUOP; }
"<="                            { yylval.intVal = OP_LE; return RELOP; }
">="                            { yylval.intVal = OP_GE; return RELOP; }
"++"                            { return INCOP; }
"--"                            { return DECOP; }
"->"                            { return STRUCTOP; }

  /* Single operators + chane yytext to real operators */
"<"                             { yylval.intVal = OP_LT; return RELOP; }
">"                             { yylval.intVal = OP_GT; return RELOP; }
"+"                             { return '+'; }
"-"                             { return '-'; }
"*"                             { return '*'; }
"/"                             { return '/'; }
"%"                             { return '%'; }
"="                             { return '='; }
"!"                             { return '!'; }
"&"                             { return '&'; }
"("                             { return '('; }
")"                             { return ')'; }
"["                             { return '['; }
"]"                             { return ']'; }
"{"                             { return '{'; }
"}"                             { return '}'; }
"."                             { return '.'; }
","                             { return ','; }
";"                             { return ';'; }

  /* Whitespaces */
{whitespace}


  /* Comment blocks */
<INITIAL,COMMENT>"/*"           {
                                  ++comment_depth;
                                  BEGIN (COMMENT);
                                }
<COMMENT>"*/"                   {
                                  if(--comment_depth == 0)
                                    BEGIN (INITIAL);
                                }
<COMMENT>.

  /* Tracking line numbers */
<*>\n                           { ++lineno; }

%%

/* User Code Section */
/* The user code section is simply copied to lex.yy.c verbatim. */

int get_lineno() {
  return lineno;
}

int main(int argc, char* argv[]) {
  /* Initialize semantic analyzer's symbol table with built-in types */
  init_type();

  /* argc 체크: input과 output 모두 필요 */
  if (argc < 3) {
    printf("Usage: %s <input.c> <output.s>\n", argv[0]);
    exit(1);
  }

  /* 입력 파일 열기 */
  filename = argv[1];
  yyin = fopen(argv[1], "r");
  if (!yyin) {
    printf("Can't open input file: %s\n", argv[1]);
    exit(1);
  }

  /* 출력 파일 열기 */
  outfile = fopen(argv[2], "w");
  if (!outfile) {
    printf("Can't open output file: %s\n", argv[2]);
    fclose(yyin);
    exit(1);
  }

  yyparse();

  /* 정리 */
  if (yyin && yyin != stdin) fclose(yyin);
  if (outfile) fclose(outfile);

  return 0;
}

int yywrap(void) {
  return 1;
}
