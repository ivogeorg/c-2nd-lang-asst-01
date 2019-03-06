//
// Created by Ivo Georgiev on 2019-02-19.
//

#include <stdio.h>


// Forward declarations
// ====================
// Functions that are called from (the functions)
// in this file have to have forward declarations,
// which are just the function signatures (their
// first lines, terminated with a trailing semicolon.
// The definitions of functions of these functions,
// that is, the full signatures with code blocks,
// may be in other source files (files with the .c
// extension) in the same project. The compiler
// needs the declarations and the linker needs
// the definitions.
void some_pointer_play_function();
void run_arrays();


int main() {
    printf("Hello, MSUD!\n");

    some_pointer_play_function();

    run_arrays();

    return 0;
}