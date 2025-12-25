# Star: A Toy C Compiler
Star is a small, lightweight compiler that implements a small subset of the C99 specification. It implements a handwritten recursive-descent parser and recursive code generation, allowing it to robustly produce systematically-valid LLVM intermediate representation from C.

## Features
### Implemented
- integer variables
- return statements
- `while` loops
- `if` statements
- All non-pointer operators

### Working On
- support for non-integer datatypes (parser written, working on generation)
- functions (parser written, working on generation)
- pointers (working on parser)
- arrays (working on parser)

### Horizon
- `struct`
- `typedef`
- `enums`
- `unions`

For a view of currently-supported features, view the packaged tests, which all return expected values. Note that `printf` is not yet supported due to lack of string + function support.

## Usage and Installation
> Note: because Star generates LLVM intermediate representation, you must have LLVM tools installed. See [here](https://llvm.org/docs/GettingStarted.html) for installation instructions, or use your system package manager (eg. `sudo apt install llvm` or `brew install llvm`)

To build, enter `src` and `make && make clean`. The Makefile assumes the presence of `clang` on the build system; to use `gcc` et. al., modify the `CC` flag accordingly. An executable named `star` will be installed in the program directory.

To compile `foo.c` into `foo.ll` (LLVM IR), execute `./star foo.c foo.ll`. To then execute `foo.ll`, assuming that LLVM tools are installed on the system, run `lli foo.ll`.

The tests in `/tests` may be automatically run by calling `./test.sh` (do `chmod +x test.sh` beforehand) assuming that LLVM tools are installed.

## Sourcemap
`src/`
- `gen.c` and `gen.h` recursively generate LLVM IR from an abstract syntax tree
- `lex.c` and `lex.h` tokenize (ie. lex) source code into a queue
- `parse.c` and `parse.h` implement a handwritten recursive descent parser to generate a robust abstract syntax tree
- `queue.c`, `queue.h`, `tree.c` and `tree.h` implement the two data structures used all over Star
- `util.c` and `util.h` implement two utility functions used in the lexer and parser.
- `star.c` implements Star's (rudimentery) front-end
- `Makefile` is a makefile

`tests/`
- `assignments.c` tests assignment operators (`=`, `+=`, `-=`, `*=`, etc.)
- `flow.c` tests control flow (`if`/`else` statements, `while` loops)
    - `if.c` tests nested `if` statement
- `parenthesis.c` tests order of operations and validates the RDP
- `unary.c` tests unary operators (`++`, `--`)

## Acknowlegements
Thanks to Dr. Termer's _MIT 6.004_ (Computational Structures, OpenCourseWare) for giving me the idea to embark on this project and to Bob Nystrom's wonderful _Crafting Interpreters_ for teaching me how to do it.