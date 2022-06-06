#include <assert.h>
#include <ctype.h>
#include <stdio.h>

int main(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        printf("[%d] = %s\n", i, argv[i]);
    }

    printf("%d\n", 583875381);
    printf("%.*s", (int)3, "Hello");

    for (char c = 'a'; c != 'z'; ++c) {
        assert(isupper(toupper(c)));
    }
    return 0;
}
