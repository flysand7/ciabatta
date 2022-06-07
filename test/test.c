#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <stdbool.h>
#include <inttypes.h>

#include <signal.h>

void my_va_handler(int a) {
    printf("NULLPTR deref or something idk not an expert in signals\n");
}

int main(int argc, char** argv) {
    signal(SIGSEGV, my_va_handler);
    signal(SIGFPE,  my_va_handler);

    int a = INT_MAX;
    a /= 0;

    return 0;
}