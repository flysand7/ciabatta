#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <stdbool.h>
#include <inttypes.h>

#include <signal.h>

void onabort(int a) {
    printf("I don't want to live anymore\n");
    exit(-69);
}

int main(int argc, char** argv) {
    signal(SIGABRT, onabort);
    assert(0 != 0);
    return 0;
}