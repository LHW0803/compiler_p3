# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is **Project 3: Semantic Analyzer** for the SubC compiler - a simplified subset of C. The goal is to implement semantic analysis including a scoped symbol table and 15 semantic checks.

**Deadline: November 30, 2025**

## Build Commands

```bash
cd intcmp-2025/project3/src

# Build the compiler
make

# Clean build artifacts
make clean

# Full rebuild
make clean && make
```

## Testing

```bash
cd intcmp-2025/project3/src

# Run individual test cases
./subc ../open_test/var_def.c    # Variable declaration/redeclaration tests
./subc ../open_test/func_op.c    # Function operation tests
./subc ../open_test/ptr_op.c     # Pointer operation tests
./subc ../open_test/str_op.c     # Struct operation tests

# Compare output
./subc ../open_test/var_def.c > output.txt
diff output.txt expected.txt
```

## Architecture

### Source Files (intcmp-2025/project3/src/)

- **subc.h** - Header with type/symbol structures (needs extension for Type, Symbol, Node structs)
- **subc.l** - Flex lexer (mostly complete, handles tokenization and line number tracking)
- **subc.y** - Bison parser (skeleton only, needs grammar rules and semantic actions)
- **hash.c** - Symbol table implementation (needs scoped hash table implementation)

### Build Pipeline

```
subc.y  --[bison -d]-->  subc.tab.c, subc.tab.h
subc.l  --[flex]------>  lex.yy.c
*.c     --[gcc]------->  subc (executable)
```

## Key Implementation Requirements

### Scoped Symbol Table
- Stack of hash tables, one per scope
- `push_scope()` on entering `{`, `pop_scope()` on leaving `}`
- Lookup searches current scope first, then outer scopes
- Structs and functions are always global scope

### 15 Semantic Checks to Implement

1. **Undeclared identifiers** - `error_undeclared()`
2. **Redeclarations** - `error_redeclaration()`
3. **Assignment lvalue** - `error_assignable()`
4. **Assignment type incompatibility** - `error_incompatible()`
5. **NULL to non-pointer** - `error_null()`
6. **Binary operators (+,-,*,/,%,&&,||)** - `error_binary()` - both operands must be int
7. **Unary operators (-,++,--,!)** - `error_unary()` - type restrictions apply
8. **Relational operators (>,>=,<,<=)** - `error_comparable()` - same type required
9. **Equality operators (==,!=)** - `error_comparable()` - includes pointer comparison
10. **Indirection (*)** - `error_indirection()` - requires pointer
11. **Address-of (&)** - `error_addressof()` - requires variable (not pointer, array, or expression)
12. **Struct member (.)** - `error_struct()`, `error_member()`
13. **Struct pointer member (->)** - `error_strurctp()`, `error_member()`
14. **Array subscript ([])** - `error_array()`, `error_subscript()`
15. **Function calls/returns** - `error_function()`, `error_arguments()`, `error_return()`

### Error Message Format

**Must be exact** (for auto-grading):
```
filename.c:42: error: <message>
```

The `error_preamble()` function needs to be implemented using `get_lineno()`.

## SubC vs Standard C Differences

- **No type coercion**: `int + char` is an error
- **Structs are always global**: cannot define struct inside function
- **Arrays are not lvalues**: `arr = ptr` is an error
- **++/-- only on int/char**: pointer increment is an error
- **& only on simple variables**: `&arr`, `&ptr`, `&*p` are all errors
- **Pointer arithmetic not allowed**: `p + 1` is an error

## Error Recovery Pattern

All Bison actions should follow this pattern:
```c
expr : expr '+' expr
{
    if ($1 == NULL || $3 == NULL) {
        $$ = NULL;  // Propagate error
    } else if (/* type check fails */) {
        error_binary();
        $$ = NULL;  // Report and propagate
    } else {
        $$ = make_node(...);  // Success
    }
}
```

## Data Structures to Add (subc.h)

```c
typedef enum { TYPE_INT, TYPE_CHAR, TYPE_VOID, TYPE_POINTER, TYPE_ARRAY, TYPE_STRUCT, TYPE_FUNC } TypeKind;
typedef enum { SYM_VAR, SYM_FUNC, SYM_STRUCT } SymbolKind;

typedef struct Type { TypeKind kind; /* union for pointer/array/struct/func data */ } Type;
typedef struct Symbol { char *name; SymbolKind kind; Type *type; int scope; } Symbol;
typedef struct Node { Type *type; int isLvalue; int isVariable; } Node;
```

## Reference Documentation

- `intcmp-2025/project3/docs/project3.pdf` - Official specification
- `intcmp-2025/project3/프로젝트_상세_분석_보고서.txt` - Detailed analysis (Korean)
