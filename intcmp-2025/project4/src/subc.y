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
#include <stdlib.h>  /* malloc */
#include <string.h>  /* strlen, memcpy */
#include "subc.h"

int   yylex ();
int   yyerror (char* s);
int   get_lineno();
char* yyget_text();
void  reduce(char* s);

/* [Project 3] Add filename variable for error messages */
char* filename;

/* Function declarations support */
static struct decl *pending_func = NULL;  /* curent function declaration passing */

/* [Project 4] 함수 코드 생성용 */
static char *current_func_name = NULL;     /* 현재 함수명 (복사본) */
static int pending_function_body = 0;      /* 다음 compound_stmt가 함수 body임 */
static int in_function_body = 0;           /* 현재 compound_stmt가 함수 body 처리 중 */
static int local_offset = 0;               /* 지역 변수 offset (fp+1부터 시작) */

/* [Phase 5] 제어흐름 라벨 스택 */
static char *if_label_stack[100];          /* if/else 라벨 스택 */
static int if_label_top = 0;

/* [Phase 5-2] 루프 라벨 스택 (break/continue 지원) */
struct loop_labels {
    char *cond_label;      /* 조건 검사 위치 (while: begin, for: cond) */
    char *continue_label;  /* continue 타겟 (while: cond, for: update) */
    char *break_label;     /* break 타겟 (루프 종료점) */
    char *body_label;      /* for 전용: body 시작 (update 건너뛰기용) */
};
static struct loop_labels loop_stack[100];
static int loop_stack_top = 0;

/* [Project 4] Phase 3.5: need_rvalue 문맥 플래그 */
static int need_rvalue = 1;                /* 기본: 값 필요 */
static int need_rvalue_stack[100];
static int need_rvalue_sp = 0;

#define SAVE_NEED_RVALUE() do { \
  if (need_rvalue_sp >= 100) { fprintf(stderr, "need_rvalue stack overflow\n"); exit(1); } \
  need_rvalue_stack[need_rvalue_sp++] = need_rvalue; \
} while(0)

#define RESTORE_NEED_RVALUE() do { \
  if (need_rvalue_sp <= 0) { fprintf(stderr, "need_rvalue stack underflow\n"); exit(1); } \
  need_rvalue = need_rvalue_stack[--need_rvalue_sp]; \
} while(0)

/* helper function: string duplicate */
static char* my_strdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = (char*)malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

/* helper function: pointer level add */
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
  struct id *idptr;      /* for ID */
  struct decl *declptr;
  struct exprinfo expr;  /* 표현식 정보 */
  struct exprinfo *exprlist;  /* 인자 리스트용 */
}

/* Tokens and Types */
/* Add more tokens and types */
%token<declptr>   TYPE
%token            STRUCT
%token            SYM_NULL RETURN IF ELSE WHILE FOR BREAK CONTINUE
%token            LOGICAL_OR LOGICAL_AND INCOP DECOP STRUCTOP
%token<intVal>    RELOP EQUOP
%token<idptr>     ID
%token<stringVal> CHAR_CONST STRING
%token<intVal>    INTEGER_CONST

/* Non-terminal types */
%type<declptr>    type_specifier struct_specifier func_decl
%type<intVal>     pointers
%type<expr>       unary binary expr assignment expr_e
%type<exprlist>   args args_opt

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
  : {
      /* Startup code - main도 함수이므로 caller 템플릿 적용 */
      emit("push_const 0");        /* return slot (main 반환값) */
      emit("push_const EXIT");     /* return address */
      emit("push_reg fp");         /* old_fp */
      emit("push_reg sp");
      emit("pop_reg fp");          /* fp = sp */
      emit("jump main");

      /* EXIT 레이블 (함수들 앞에 위치) */
      emit_label("EXIT");
      emit("exit");
    }
    ext_def_list
    {
      /* Lglob 및 문자열 테이블 */
      emit("Lglob. data %d", global_words);
      emit_string_table();

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
      /* type error propagation */
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
        /* [Project 4] 전역 변수 offset 할당 */
        vardecl->offset = global_words;
        vardecl->is_global = 1;
        global_words += size_in_words(vardecl);

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
        /* [Project 4] 전역 배열 offset 할당 */
        constdecl->offset = global_words;
        constdecl->is_global = 1;
        global_words += size_in_words(constdecl);

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
  | func_decl
    {
      /* [Project 4] 함수 정의에서만 레이블/pending 설정 */
      if (current_func_name != NULL) {
        emit_label(current_func_name);
        emitted_any_function = 1;
        pending_function_body = 1;
      }
    }
    compound_stmt
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
    /* struct 재정의를 먼저 검사 */
    if (findstructdecl($2) != NULL) {
      error_redeclaration();
    }
    push_scope();  /* 필드 전용 */
  }
  def_list '}'
  {
    struct ste *fields = pop_scope();  /* 필드 리스트 수집 */
    
    /* if error -> not entry */
    if (findstructdecl($2) != NULL) {
      $$ = NULL;
    } else {
      struct decl *structdecl = makestructdecl(fields);
      /* struct -> parrent scope */
      declare_struct($2, structdecl);
      $$ = structdecl;
    }
    REDUCE("struct_specifier->STRUCT ID '{' def_list '}'");
  }
  | STRUCT ID
  {
    /* type using -> lookup -> all scope search */
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
    pending_func = NULL;  /* prev function clear */
    
    if ($1 == NULL) {
      $$ = NULL;  /* type error prop */
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

          /* [Project 4] 함수명 저장 */
          if (current_func_name) free(current_func_name);
          current_func_name = my_strdup($3->name);

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

      /* [Project 4] 함수명 저장 */
      if (current_func_name) free(current_func_name);
      current_func_name = my_strdup($3->name);
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
      funcdecl->formals = formals;  /* 순수 매개변수 리스트 (reverse) */
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
      /* type 에러 전파 - declare x */
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
        /* [Project 4] 지역 변수 offset 할당 */
        vardecl->offset = local_offset;
        vardecl->is_global = 0;
        local_offset += size_in_words(vardecl);

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
        /* [Project 4] 지역 배열 offset 할당 */
        constdecl->offset = local_offset;
        constdecl->is_global = 0;
        local_offset += size_in_words(constdecl);

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

    /* [Project 4] 이 compound_stmt가 함수 body인지 저장 */
    $<intVal>$ = pending_function_body;

    /* [Project 4] 함수 본문 시작 처리 */
    if (pending_function_body) {
      in_function_body = 1;
      pending_function_body = 0;

      /* main_start: 레이블 */
      char start_label[128];
      snprintf(start_label, 128, "%s_start", current_func_name);
      emit_label(start_label);

      /* [Project 4] 지역 변수 offset 초기화 */
      local_offset = 1;  /* fp+1부터 시작 */
    }

    /* 함수 body인 경우 returnid와 매개변수들을 새로 declare */
    if (pending_func != NULL) {
      /* 1) returnid 설치 */
      declare(returnid, pending_func->returntype);

      /* 현재 함수의 반환 타입 추적 */
      current_func_ret_type = pending_func->returntype;

      /* 2) formals 리스트 순회하며 매개변수 설치
       * 주의: pop_scope()가 리스트를 뒤집으므로 formals는 정순 (a→b→c)
       * 스택 레이아웃: arg_0(a)이 가장 먼저 push되어 fp-2-n에 위치
       *              arg_{n-1}(c)이 마지막 push되어 fp-3에 위치 */
      /* [Project 4] 매개변수 offset 설정 */

      /* 먼저 매개변수 개수 세기 */
      int num_params = 0;
      struct ste *p = pending_func->formals;
      while (p != NULL) { num_params++; p = p->prev; }

      /* offset 할당: arg_0은 fp-(2+n), arg_{n-1}은 fp-3 */
      int param_offset = -(2 + num_params);  /* arg_0부터 시작 */
      p = pending_func->formals;
      while (p != NULL) {
        p->decl->offset = param_offset;
        p->decl->is_global = 0;  /* 지역 변수 */

        /* 새로운 ste를 body scope에 생성 */
        declare(p->name, p->decl);

        param_offset++;  /* 증가 (arg_0 → arg_1 → ... → arg_{n-1}) */
        p = p->prev;
      }

      pending_func = NULL;  /* 사용 완료 */
    }
  }
  def_list
  {
    /* [Project 4] Mid-rule: emit shift_sp for local variables after def_list */
    /* $<intVal>2: 첫 번째 mid-rule action의 값 (함수 body 여부) */
    if ($<intVal>2) {
      int num_locals = local_offset - 1;  /* -1 because we started at 1 */
      if (num_locals > 0) {
        /* 스택이 위로 자람 (PUSH: sp++). 공간 확보는 sp 증가이므로 양수 사용 */
        emit("shift_sp %d", num_locals);
      }
    }
  }
  stmt_list '}'
  {
    /* [Project 4] 함수 본문 종료 처리 */
    /* $<intVal>2: 첫 번째 mid-rule action의 값 (함수 body 여부) */
    if ($<intVal>2) {
      char final_label[128], end_label[128];
      snprintf(final_label, 128, "%s_final", current_func_name);
      snprintf(end_label, 128, "%s_end", current_func_name);

      emit_label(final_label);
      emit("push_reg fp");
      emit("pop_reg sp");
      emit("pop_reg fp");
      emit("pop_reg pc");
      emit_label(end_label);

      current_func_ret_type = NULL;
      in_function_body = 0;
    }

    pop_scope();

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
    /* [Project 4] Discard expression result */
    /* void 타입이면 스택에 값이 없으므로 shift_sp 생략 */
    if ($1.type != NULL && $1.type != voidtype) {
      emit("shift_sp -1");
    }

    REDUCE("stmt->expr ';'");
  }
  | compound_stmt
  {
    REDUCE("stmt->compound_stmt");
  }
  | RETURN ';'
  {
    /* current_func_ret_type 사용 */
    if (current_func_ret_type != NULL) {
      if (current_func_ret_type != voidtype) {
        error_return();  /* void가 아닌 함수에서 값 없이 return */
      }
    }
    /* [Project 4] void return: 바로 종료 라벨로 점프 */
    if (current_func_name != NULL) {
      emit("jump %s_final", current_func_name);
    }
    /* current_func_ret_type == NULL: 함수 밖 return - 조용히 무시 */
    REDUCE("stmt->RETURN ';'");
  }
  | RETURN
  {
    /* [Project 4] Mid-rule: return slot 주소를 먼저 생성
     * 스택 순서: [..., addr] → expr 평가 후 [..., addr, value] */
    if (current_func_name != NULL) {
      emit("push_reg fp");
      emit("push_const -2");
      emit("add");  /* stack top = fp-2 (return slot) */
    }
  }
  expr ';'
  {
    /* current_func_ret_type 사용 */
    /* 에러 전파: $3.type == NULL이면 expr에서 이미 에러 */
    if (current_func_ret_type != NULL && $3.type != NULL) {
      if (current_func_ret_type == voidtype) {
        error_return();  /* void 함수에서 값 반환 */
      } else if (!check_same_type(current_func_ret_type, $3.type)) {
        error_return();  /* 타입 불일치 */
      }
    }
    /* [Project 4] 반환값을 return slot에 저장하고 종료 라벨로 점프 */
    if (current_func_name != NULL) {
      emit("assign");  /* mem[fp-2] = value */
      emit("jump %s_final", current_func_name);
    }
    /* current_func_ret_type == NULL: 함수 밖 return - 조용히 무시 */
    REDUCE("stmt->RETURN expr ';'");
  }
  | ';'
  {
    REDUCE("stmt->';'");
  }
  | if_head stmt %prec LOWER_THAN_ELSE
    {
      /* [Phase 5-1] Simple if: emit else_label */
      emit_label(if_label_stack[--if_label_top]);
      REDUCE("stmt->IF '(' expr ')' stmt");
    }
  | if_head stmt ELSE
    {
      /* [Phase 5-1] if-else: emit jump to end, emit else_label, push end_label */
      char *end_label = new_label("Lend_");
      emit("jump %s", end_label);
      emit_label(if_label_stack[--if_label_top]);
      if_label_stack[if_label_top++] = end_label;
    }
    stmt
    {
      /* Emit end_label */
      emit_label(if_label_stack[--if_label_top]);
      REDUCE("stmt->IF '(' expr ')' stmt ELSE stmt");
    }
  | while_head stmt
    {
      /* [Phase 5-2] 루프 끝: jump back + end label */
      emit("jump %s", loop_stack[loop_stack_top-1].continue_label);
      emit_label(loop_stack[loop_stack_top-1].break_label);
      loop_stack_top--;
      REDUCE("stmt->WHILE '(' expr ')' stmt");
    }
  | for_head expr_e
    {
      /* [Phase 5-3] update expr 결과 버림 (있으면), jump cond, body 라벨 */
      if ($2.type != NULL) {
        emit("shift_sp -1");
      }
      emit("jump %s", loop_stack[loop_stack_top-1].cond_label);
      emit_label(loop_stack[loop_stack_top-1].body_label);
    }
    ')' stmt
    {
      /* body 끝: jump update, end 라벨 */
      emit("jump %s", loop_stack[loop_stack_top-1].continue_label);
      emit_label(loop_stack[loop_stack_top-1].break_label);
      loop_stack_top--;
      REDUCE("stmt->FOR '(' expr_e ';' expr_e ';' expr_e ')' stmt");
    }
  | BREAK ';'
  {
    /* [Phase 5-4] break: 현재 루프의 break_label로 점프 */
    if (loop_stack_top > 0) {
      emit("jump %s", loop_stack[loop_stack_top-1].break_label);
    }
    REDUCE("stmt->BREAK ';'");
  }
  | CONTINUE ';'
  {
    /* [Phase 5-4] continue: 현재 루프의 continue_label로 점프 */
    if (loop_stack_top > 0) {
      emit("jump %s", loop_stack[loop_stack_top-1].continue_label);
    }
    REDUCE("stmt->CONTINUE ';'");
  }
  ;

/* [Phase 5-1] if 문 접두사 - 공통 factoring으로 reduce-reduce 충돌 해결 */
if_head
  : IF '(' expr ')'
    {
      char *label = new_label("Lelse_");
      if_label_stack[if_label_top++] = label;
      emit("branch_false %s", label);
    }
  ;

/* [Phase 5-2] while 문 접두사 */
while_head
  : WHILE '('
    {
      /* 루프 시작 라벨 생성 및 emit */
      char *begin_label = new_label("Lwhile_");
      char *end_label = new_label("Lend_");
      loop_stack[loop_stack_top].cond_label = begin_label;
      loop_stack[loop_stack_top].continue_label = begin_label;  /* while: continue→조건 */
      loop_stack[loop_stack_top].break_label = end_label;
      loop_stack[loop_stack_top].body_label = NULL;  /* while은 body_label 미사용 */
      loop_stack_top++;
      emit_label(begin_label);
    }
    expr ')'
    {
      /* 조건 평가 후 branch_false */
      emit("branch_false %s", loop_stack[loop_stack_top-1].break_label);
    }
  ;

/* [Phase 5-3] for 문 접두사 */
for_head
  : FOR '(' expr_e ';'
    {
      /* init expr 결과 버림 (있으면) */
      if ($3.type != NULL) {
        emit("shift_sp -1");
      }
      /* 라벨 4개 생성 */
      char *cond_label = new_label("Lcond_");
      char *update_label = new_label("Lupdate_");
      char *body_label = new_label("Lbody_");
      char *end_label = new_label("Lend_");
      loop_stack[loop_stack_top].cond_label = cond_label;
      loop_stack[loop_stack_top].continue_label = update_label;  /* for: continue→update */
      loop_stack[loop_stack_top].break_label = end_label;
      loop_stack[loop_stack_top].body_label = body_label;
      loop_stack_top++;
      /* cond 라벨 emit */
      emit_label(cond_label);
    }
    expr_e ';'
    {
      /* cond expr 처리 (없으면 push_const 1) */
      if ($6.type == NULL) {
        emit("push_const 1");
      }
      emit("branch_false %s", loop_stack[loop_stack_top-1].break_label);
      emit("jump %s", loop_stack[loop_stack_top-1].body_label);
      /* update 라벨 emit - 여기서부터 update 코드 */
      emit_label(loop_stack[loop_stack_top-1].continue_label);
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
  | unary
    {
      /* [Project 4] Mid-rule: LHS 주소 복제 */
      emit("push_reg sp");
      emit("fetch");
    }
    '=' assignment
  {
    /* [Project 4] RHS는 이미 r-value (binary:unary에서 처리됨) */
    emit("assign");
    emit("fetch");  /* assignment result */

    /* Semantic analysis: LHS=$1, RHS=$4 */
    if ($1.type == NULL || ($4.type == NULL && !$4.is_null_const)) {
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if (!$1.is_lvalue) {
      error_assignable();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else if ($4.is_null_const) {
      if (!is_pointer_type($1.type)) {
        error_null();
        $$.type = NULL;
        $$.is_lvalue = 0;
        $$.is_var = 0;
        $$.is_null_const = 0;
      } else {
        $$.type = $1.type;
        $$.is_lvalue = 0;
        $$.is_var = 0;
        $$.is_null_const = 0;
      }
    } else if (!check_same_type($1.type, $4.type)) {
      error_incompatible();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      $$.type = $1.type;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
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
      /* [Project 4] 비교 연산 */
      switch ($2) {
        case OP_LT: emit("less"); break;
        case OP_LE: emit("less_equal"); break;
        case OP_GT: emit("greater"); break;
        case OP_GE: emit("greater_equal"); break;
      }
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
      /* [Project 4] 동등 비교 */
      switch ($2) {
        case OP_EQ: emit("equal"); break;
        case OP_NE: emit("not_equal"); break;
      }
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
      /* [Project 4] 덧셈 */
      emit("add");
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
      /* [Project 4] 뺄셈 */
      emit("sub");
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
      /* [Project 4] 곱셈 */
      emit("mul");
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
      /* [Project 4] 나눗셈 */
      emit("div");
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
      /* [Project 4] 나머지 */
      emit("mod");
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
      /* [Project 4] 논리 AND */
      emit("and");
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
      /* [Project 4] 논리 OR */
      emit("or");
      $$.type = inttype;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    }
    REDUCE("binary->binary LOGICAL_OR binary");
  }
  | unary %prec '='
  {
    /* [Project 4] l-value → r-value 변환
     * need_rvalue=1이고 l-value면 fetch하여 값으로 변환
     */
    $$ = $1;
    if (need_rvalue && $$.is_lvalue) {
      emit("fetch");
      $$.is_lvalue = 0;
    }
    REDUCE("binary->unary");
  }
  ;

unary
  : '(' { SAVE_NEED_RVALUE(); } expr ')'
  {
    RESTORE_NEED_RVALUE();
    $$ = $3;  /* $2는 mid-rule, expr은 $3 */
    REDUCE("unary->'(' expr ')'");
  }
  | INTEGER_CONST
  {
    /* [Project 4] Emit constant value */
    emit("push_const %d", $1);

    $$.type = inttype;
    $$.is_lvalue = 0;
    $$.is_var = 0;
    $$.is_null_const = 0;
    $$.next = NULL;
    REDUCE("unary->INTEGER_CONST");
  }
  | CHAR_CONST
  {
    /* DEBUG: Print CHAR_CONST type */
    /* fprintf(stderr, "DEBUG: CHAR_CONST - chartype=%p (declclass=%d, typeclass=%d)\n",
            (void*)chartype, chartype ? chartype->declclass : -1,
            chartype ? chartype->typeclass : -1); */
    $$.type = chartype;
    $$.is_lvalue = 0;
    $$.is_var = 0;
    $$.is_null_const = 0;
    $$.next = NULL;
    REDUCE("unary->CHAR_CONST");
  }
  | STRING
  {
    /* [Project 4] 문자열 리터럴: 테이블에 추가하고 라벨 push */
    char *label = add_string($1);
    emit("push_const %s", label);

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
      /* [Project 4] Emit l-value (address) for variable */
      if (d->is_global) {
        emit("push_const Lglob+%d", d->offset);
      } else {
        /* local variable: fp + offset */
        emit("push_reg fp");
        emit("push_const %d", d->offset);
        emit("add");
      }

      struct decl *t = get_type(d);
      $$.type = t;
      $$.is_lvalue = 1;

      /* & 연산자는 포인터와 배열에 대해서만 금지 */
      if (is_pointer_type(t) || is_array_type(t)) {
        $$.is_var = 0;  /* &p, &arr 금지 */
      } else {
        $$.is_var = 1;  /* int, char, struct 허용 */
      }

      $$.is_null_const = 0;
      $$.next = NULL;
    } else if (d->declclass == DECL_CONST) {
      /* [Project 4] Array: emit base address as r-value */
      if (d->is_global) {
        emit("push_const Lglob+%d", d->offset);
      } else {
        emit("push_reg fp");
        emit("push_const %d", d->offset);
        emit("add");
      }

      $$.type = d->type;
      $$.is_lvalue = 0;   /* 배열은 lvalue 아님 */
      $$.is_var = 0;      /* &arr 불가 */
      $$.is_null_const = 0;
      $$.next = NULL;
    } else if (d->declclass == DECL_FUNC) {
      /* 함수 이름은 함수 decl 자체를 type으로 - 코드 생성 없음 */
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
    /* [Project 4] Post-increment: result = old value
     * Stack before: [addr]
     * Pattern: dup addr twice, fetch old, add 1, assign, fetch new, sub 1
     */
    emit("push_reg sp");
    emit("fetch");      /* [addr, addr] */
    emit("push_reg sp");
    emit("fetch");      /* [addr, addr, addr] */
    emit("fetch");      /* [addr, addr, old] */
    emit("push_const 1");
    emit("add");        /* [addr, addr, new] */
    emit("assign");     /* [addr] */
    emit("fetch");      /* [new] */
    emit("push_const 1");
    emit("sub");        /* [old] - result */

    /* Semantic analysis */
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
      $$.type = $1.type;
      $$.is_lvalue = 0;  /* result is r-value */
      $$.is_var = 0;
      $$.is_null_const = 0;
    }
    REDUCE("unary->unary INCOP");
  }
  | unary DECOP
  {
    /* [Project 4] Post-decrement: result = old value
     * Stack before: [addr]
     * Pattern: dup addr twice, fetch old, sub 1, assign, fetch new, add 1
     */
    emit("push_reg sp");
    emit("fetch");      /* [addr, addr] */
    emit("push_reg sp");
    emit("fetch");      /* [addr, addr, addr] */
    emit("fetch");      /* [addr, addr, old] */
    emit("push_const 1");
    emit("sub");        /* [addr, addr, new] */
    emit("assign");     /* [addr] */
    emit("fetch");      /* [new] */
    emit("push_const 1");
    emit("add");        /* [old] - result */

    /* Semantic analysis */
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
      $$.type = $1.type;
      $$.is_lvalue = 0;  /* result is r-value */
      $$.is_var = 0;
      $$.is_null_const = 0;
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
    /* [Project 4] 포인터 역참조
     * - operand가 l-value면 fetch로 포인터 값 획득
     * - operand가 r-value면 이미 포인터 값이므로 그대로
     * - 결과는 항상 l-value (포인터 값이 새 주소가 됨)
     * - semantic error 경로에서는 emit 하지 않음
     */
    if ($2.type == NULL) {
      $$ = $2;
    } else if (!is_pointer_type($2.type)) {
      error_indirection();
      $$.type = NULL;
      $$.is_lvalue = 0;
      $$.is_var = 0;
      $$.is_null_const = 0;
    } else {
      /* 정상 경로: 코드 생성 */
      if ($2.is_lvalue) {
        emit("fetch");  /* 포인터 값 획득 */
      }
      /* 결과: 스택에 포인터 값(=주소)이 있고, 이것이 *E의 l-value */

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
    /* [Project 4] 배열 접근 코드 생성
     * Stack before: [base_addr, index_value_or_addr]
     * 계산: base + index * element_size
     * Stack after: [element_addr] (l-value)
     * - semantic error 경로에서는 emit 하지 않음
     */
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
      /* 정상 경로: 코드 생성 */
      struct decl *elem = get_type($1.type)->elementvar;

      /* elem == NULL 방어 */
      if (!elem) {
        $$.type = NULL;
        $$.is_lvalue = 0;
        $$.is_var = 0;
        $$.is_null_const = 0;
      } else {
        int elem_size = size_in_words(elem);

        /* index가 l-value면 fetch하여 값으로 변환 */
        if ($3.is_lvalue) {
          emit("fetch");
        }

        /* element size가 1이 아니면 곱셈 */
        if (elem_size != 1) {
          emit("push_const %d", elem_size);
          emit("mul");
        }

        /* base + offset = element 주소 */
        emit("add");

        /* 결과는 l-value */
        $$.type = elem->type;
        $$.is_lvalue = 1;  /* arr[i]는 lvalue */
        $$.is_var = 1;     /* &arr[i] 가능 */
        $$.is_null_const = 0;
      }
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
  | ID '(' args_opt ')'
  {
    /* [Project 4] 함수 호출: ID로 직접 호출 */
    struct id *fname = $1;
    struct decl *callee = lookup(fname);

    /* 인자 리스트에 에러가 있는지 확인 */
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
        $$.type = NULL;
        $$.is_lvalue = 0;
        $$.is_var = 0;
        $$.is_null_const = 0;
        $$.next = NULL;
    } else if (callee == NULL) {
        error_undeclared();
        $$.type = NULL;
        $$.is_lvalue = 0;
        $$.is_var = 0;
        $$.is_null_const = 0;
        $$.next = NULL;
    } else if (callee->declclass != DECL_FUNC) {
        error_function();
        $$.type = NULL;
        $$.is_lvalue = 0;
        $$.is_var = 0;
        $$.is_null_const = 0;
        $$.next = NULL;
    } else if (!check_function_arguments(callee, $3)) {
        error_arguments();
        $$.type = NULL;
        $$.is_lvalue = 0;
        $$.is_var = 0;
        $$.is_null_const = 0;
        $$.next = NULL;
    } else {
        /* 호출 성공 */
        /* [Project 4] built-in 함수 처리 */
        if (strcmp(fname->name, "write_string") == 0) {
            emit("write_string");
        } else if (strcmp(fname->name, "write_int") == 0) {
            emit("write_int");
        } else if (strcmp(fname->name, "write_char") == 0) {
            emit("write_char");
        } else if (strcmp(fname->name, "read_int") == 0) {
            emit("read_int");
        } else if (strcmp(fname->name, "read_char") == 0) {
            emit("read_char");
        } else {
            /* 사용자 정의 함수: caller 템플릿 */
            /* (1) args는 이미 args 규칙에서 push됨 */

            /* 인자 개수 계산 */
            int nargs = 0;
            struct exprinfo *arg_p = $3;
            while (arg_p != NULL) { nargs++; arg_p = arg_p->next; }

            /* (2) return slot 확보 */
            emit("push_const 0");

            /* (3) return address - 고유 라벨 생성 */
            char *ret_label = new_label("Lret_");
            emit("push_const %s", ret_label);

            /* (4) old_fp 저장 */
            emit("push_reg fp");

            /* (5)-(6) fp = sp */
            emit("push_reg sp");
            emit("pop_reg fp");

            /* (7) jump to callee */
            emit("jump %s", fname->name);

            /* (8) 복귀 지점 */
            emit_label(ret_label);

            /* ★ args 정리 정책 ★
             * 스택이 위로 자람 (PUSH: sp++)
             * 복귀 직후 스택: arg0 ... arg_{n-1} return_value <-- sp
             * arg0 위치 = sp - nargs (더 낮은 주소)
             * 비void: return_value를 arg0 위치로 이동 후 shift
             * void: return_slot + args 모두 정리 */
            if (callee->returntype != voidtype) {
                /* 비void 함수: return_value 보존하면서 args 정리 */
                if (nargs > 0) {
                    /* 1. arg0 addr 계산: sp - nargs */
                    emit("push_reg sp");
                    emit("push_const %d", nargs);
                    emit("sub");  /* addr of arg0 = sp - nargs */

                    /* 2. return_value fetch: addr는 현재 sp - 1 */
                    emit("push_reg sp");
                    emit("push_const 1");
                    emit("sub");  /* addr of return_value */
                    emit("fetch");  /* get return_value */

                    /* 3. assign: [addr, value] 순서 */
                    emit("assign");  /* mem[arg0] = return_value */

                    /* 4. sp를 arg0 위치로 이동 (스택 되감기) */
                    emit("shift_sp %d", -nargs);
                }
                /* nargs == 0: return_value가 이미 유일한 값 */
            } else {
                /* void 함수: return_slot + args 모두 정리 (sp 감소) */
                emit("shift_sp %d", -(nargs + 1));
            }
        }

        $$.type = callee->returntype;
        $$.is_lvalue = 0;
        $$.is_var = 0;
        $$.is_null_const = 0;
        $$.next = NULL;
    }
    REDUCE("unary->ID '(' args_opt ')'");
  }
  | SYM_NULL
  {
    /* [Project 4] NULL = 0 */
    emit("push_const 0");

    $$.type = inttype;  /* NULL은 일단 int로 취급 */
    $$.is_lvalue = 0;
    $$.is_var = 0;
    $$.is_null_const = 1;  /* semantic에서 포인터 대입 허용 */
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
      /* 추가 인자: 새 노드를 만들어 끝에 붙이기 (정순 리스트) */
      struct exprinfo *node = (struct exprinfo *)malloc(sizeof(struct exprinfo));
      *node = $3;
      node->next = NULL;
      
      /* 리스트 끝까지 가서 추가 */
      struct exprinfo *p = $1;
      while (p->next != NULL) {
          p = p->next;
      }
      p->next = node;
      
      $$ = $1;  /* head는 그대로 유지 */

      REDUCE("args->args ',' expr");
  }
  ;

args_opt
  : /* empty */
  {
      $$ = NULL;
      REDUCE("args_opt->epsilon");
  }
  | args
  {
      $$ = $1;
      REDUCE("args_opt->args");
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
