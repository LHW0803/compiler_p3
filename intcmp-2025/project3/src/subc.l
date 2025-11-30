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
static int comment_depth = 0;
static int lineno = 1;
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
"=="                            { return EQUOP; }
"!="                            { return EQUOP; }
"<="                            { return RELOP; }
">="                            { return RELOP; }
"++"                            { return INCOP; }
"--"                            { return DECOP; }
"->"                            { return STRUCTOP; }

  /* Single operators + chane yytext to real operators */
"<"                             { return RELOP; }
">"                             { return RELOP; }
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
  // Initialize semantic analyzer's symbol table with built-in types
  init_type();

  // I/O stream settings
  if (argc >= 2) {
    filename = argv[1];
    yyin = fopen(argv[1], "r");
  } else {
    filename = "stdin";
    yyin = stdin;
  }

  if(!yyin) {
    printf("Can't open input stream!\n");
    exit(1);
  }

  yyparse();
  fclose(yyin);

  return 0;
}
