# Phase 4: 함수 호출 코드 생성 요약

## 1. VM 스택 특성 (중요!)

```
스택이 **위로** 자람 (PUSH: sp++, POP: sp--)
- push: sp++, stack[sp] = value
- pop: value = stack[sp], sp--
- shift_sp n: sp += n (양수=축소, 음수=확장)
- fetch: POP(addr), PUSH(stack[addr])
- assign: POP(value), POP(addr), stack[addr] = value
```

## 2. 스택 프레임 레이아웃

```
높은 주소 (sp 증가 방향)
┌─────────────────────┐
│  local_{m-1}        │ ← fp + m
│  ...                │
│  local_0            │ ← fp + 1   ★ locals 시작점
├─────────────────────┤
│  old_fp             │ ← fp + 0   ★ fp가 가리키는 위치
├─────────────────────┤
│  return address     │ ← fp - 1
├─────────────────────┤
│  return slot        │ ← fp - 2   ★ 반환값 저장
├─────────────────────┤
│  arg_{n-1}          │ ← fp - 3   (마지막 push된 arg)
│  arg_{n-2}          │ ← fp - 4
│  ...                │
│  arg_0              │ ← fp - 2 - n (첫 번째 push된 arg)
└─────────────────────┘
낮은 주소
```

## 3. 코드 생성 템플릿

### 3.1 Startup Code (program 규칙)
```asm
push_const 0        ; main용 return slot
push_const EXIT     ; return address
push_reg fp         ; old_fp
push_reg sp
pop_reg fp          ; fp = sp
jump main
EXIT:
exit
```

### 3.2 Callee - Prologue (compound_stmt)
```asm
func_name:
func_name_start:
shift_sp <locals_words>    ; 지역변수 공간 확보 (양수!)
```

### 3.3 Callee - Epilogue (compound_stmt 끝)
```asm
func_name_final:
push_reg fp
pop_reg sp
pop_reg fp
pop_reg pc
func_name_end:
```

### 3.4 return expr; (RETURN expr ';' 규칙)
```asm
; Mid-rule: addr 먼저 (assign은 [addr, value] 순서)
push_reg fp
push_const -2
add                 ; addr = fp - 2

; expr 평가 (value가 스택 top에)
<expr r-value>

; 저장 및 종료
assign              ; stack[fp-2] = value
jump func_name_final
```

### 3.5 Caller - 함수 호출 (ID '(' args_opt ')' 규칙)
```asm
; (1) args는 이미 args 규칙에서 push됨

; (2) return slot
push_const 0

; (3) return address
push_const Lret_N

; (4) old_fp
push_reg fp

; (5)-(6) fp = sp
push_reg sp
pop_reg fp

; (7) jump
jump func_name

; (8) 복귀 지점
Lret_N:

; (9) args 정리 - 스택이 위로 자라므로 sub 사용!
; 비void: return_value를 arg0 위치로 이동
push_reg sp
push_const nargs
sub                 ; addr of arg0 = sp - nargs

push_reg sp
push_const 1
sub                 ; addr of return_value
fetch

assign              ; stack[arg0] = return_value
shift_sp -nargs     ; sp를 arg0 위치로 이동 (음수!)

; void: 모두 정리
shift_sp -(nargs + 1)
```

## 4. 매개변수 Offset 할당

**핵심**: pop_scope()가 리스트를 뒤집으므로 formals는 **정순** (a→b→c)

```c
/* 매개변수 개수 세기 */
int num_params = 0;
struct ste *p = pending_func->formals;
while (p != NULL) { num_params++; p = p->prev; }

/* offset 할당: arg_0은 fp-(2+n), arg_{n-1}은 fp-3 */
int param_offset = -(2 + num_params);
p = pending_func->formals;
while (p != NULL) {
    p->decl->offset = param_offset;
    param_offset++;  // 증가!
    p = p->prev;
}
```

예시: test(int a, int b, int c)
- formals: a → b → c
- a.offset = -5, b.offset = -4, c.offset = -3
- 스택: arg0(a)=fp-5, arg1(b)=fp-4, arg2(c)=fp-3

## 5. Built-in 함수

| 함수 | opcode | 반환 타입 |
|------|--------|-----------|
| read_int() | read_int | int |
| read_char() | read_char | char |
| write_int(n) | write_int | void |
| write_char(c) | write_char | void |
| write_string(s) | write_string | void |

- Built-in은 caller frame 없이 직접 opcode emit
- void 함수 호출 후 stmt에서 shift_sp -1 생략

## 6. 수정된 파일 및 위치

### subc.y 주요 수정
| 위치 | 내용 |
|------|------|
| line 117-128 | Startup code (return slot 추가) |
| line 560-577 | 매개변수 offset 할당 (정순 처리) |
| line 583-585 | shift_sp locals (양수) |
| line 652-680 | return 문 코드 생성 |
| line 1558-1619 | 함수 호출 caller 템플릿 |

### symtab.c
| 위치 | 내용 |
|------|------|
| line 347-392 | Built-in 함수 등록 (init_type) |

### subc.l
| 위치 | 내용 |
|------|------|
| line 85-111 | add_string() 함수 |
| line 113-118 | emit_string_table() 함수 |

## 7. 발견 및 수정된 버그

| 버그 | 원인 | 수정 |
|------|------|------|
| invalid address -1 | main startup에 return slot 누락 | `push_const 0` 추가 |
| 잘못된 계산 결과 | args 정리 시 `sp + nargs` | `sp - nargs`로 수정 (sub) |
| shift_sp 방향 오류 | 스택이 위로 자라는 것 미고려 | 양수/음수 방향 수정 |
| 매개변수 순서 오류 | formals가 역순이라고 잘못 가정 | 정순 처리로 수정 |

## 8. 테스트 결과

### Basic Tests
- t1.c ~ t4.c: ✅ 모두 통과
- input.c: ✅ "hello world" 출력

### Advanced Tests
- func1.c: ✅ 20 출력 (포인터 매개변수)
- func2.c: ✅ 1 출력 (3인자 함수)
- func3.c: ✅ 10 출력 (값 출력 함수)

### Custom Tests
- myadd(3, 5): ✅ 8 출력
- 다중 호출 (3, 7, 10): ✅ SP 누적 없음

## 9. 다음 단계

Phase 5: 제어 흐름 (if, while, for, break, continue)
- branch_true, branch_false 명령어 활용
- 라벨 생성 및 관리
