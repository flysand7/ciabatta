
#include <assert.h>

#include <stdio.h>  // printf
#include <stdlib.h> // abort

void _Noreturn _assert(
    char const *cond,
    char const *func,
    char const *file,
    int line
) {
    printf("Assertion failed: %s\n", cond);
    printf("  Function: %s\n", func);
    printf("  File: %s\n", file);
    printf("  Line: %d\n", line);
    abort();
}
