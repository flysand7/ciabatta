#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <stdbool.h>
#include <inttypes.h>

#include <string.h>
#include <signal.h>

int test() {
    static int a = 2;
    a += 1;
    return a;
}

void wack() {
    printf("BYE!!!\n");
}

int main(int argc, char** argv) {
    atexit(wack);

    char* path_env = getenv("PATH");
    char *ctx, *path_token = strtok_r(path_env, ";", &ctx);
    while(path_token) {
        printf("%s\n", path_token);
        path_token = strtok_r(NULL, ";", &ctx);
    }

    test();
    char input[] = "A bird came down the walk";
    printf("Parsing the input string '%s'\n", input);
    char *token = strtok(input, " ");
    while(token) {
        printf("%s\n", token);
        token = strtok(NULL, " ");
    }

    printf("Contents of the input string now: '");
    for(size_t n = 0; n < sizeof input; ++n)
        input[n] ? printf("%c", input[n]) : printf("\\0");
    printf("'");
    return 0;
}
