#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <_compiler.h>
#include <_os.h>

// TODO: use abort() to break

extern void _assert_error(
    char *cond,
    char const *func,
    char const *file,
    char const *line
) {
    printf("Assertion failed: %s\n", cond);
    if(*func != 0) {
        printf("\tFunction: %s\n", func);
    }
    printf("\tFile: %s\n", file);
    printf("\tLine: %s\n", line);
    abort();
}
