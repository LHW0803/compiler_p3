# Step 3: init_type() 호출 통합

## 변경된 파일
- `subc.h` (filename extern 선언 추가)
- `subc.l` (main 함수 수정)

## subc.h 변경 내용

### filename extern 선언 추가 (line 93)
```c
extern char *filename;  /* filename for error messages */
```

## subc.l 변경 내용

### main 함수 수정 (line 134-156)

#### 제거된 코드
```c
// keyword 초기화 코드 제거 (lexer가 이미 처리)
char *keyword[] = { "int", NULL };
int tokentype[] = { TYPE, 0 };
for(int i=0; keyword[i] != NULL; i++) {
  enter(tokentype[i], keyword[i], strlen(keyword[i]));
}
```

#### 추가된 코드
1. `init_type()` 호출 - semantic analyzer용 built-in 타입 등록
2. `filename` 설정 - error message용

```c
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
  
  // ... 기존 코드 유지 ...
}
```

## 핵심 개념 정리
- **keyword 초기화**: lexer 계층, "int"를 TYPE 토큰으로 (이미 불필요)
- **init_type()**: semantic 계층, symbol table에 타입 선언 등록
- 두 기능은 다른 계층에서 동작

## 컴파일 테스트
- Docker 환경에서 성공적으로 컴파일됨
- 간단한 테스트 프로그램 실행 성공

## Phase 1 완료
모든 Phase 1 작업이 완료되었습니다:
1. ✅ subc.h에 구조체 추가
2. ✅ symtab.c 구현
3. ✅ init_type() 호출 통합

## 다음 단계
Phase 2: TYPE을 ID로 변환하는 작업 (나중에)