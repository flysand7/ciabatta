#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        printf("[%d] = %s\n", i, argv[i]);
    }

    printf("%d\n", 583875381);
    printf("%.*s", (int)3, "Hello");

    for (char c = 'a'; c != 'z'; ++c) {
        assert(isupper(toupper(c)));
    }

    double v0 = strtod("0X1.BC70A3D70A3D7P+6", NULL);

    // parsing with error handling
    const char *p = "111.11 -2.22 Nan nan(2) inF 0X1.BC70A3D70A3D7P+6  1.18973e+4932zzz";

    char *end;
    for (double f = strtod(p, &end); p != end; f = strtod(p, &end)) {
        printf("'%.*s' -> ", (int)(end - p), p);
        p = end;
        if (errno == ERANGE){
            printf("range error, got ");
        }

        printf("%f\n", f);
    }

    // parsing without error handling
    double v1 = strtod("  -0.0000000123junk", NULL);
    double v2 = strtod("junk", NULL);
}