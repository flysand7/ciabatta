#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    for (int i = 0; i < argv; i++) {
        printf("[%d] = %s\n", i, argv[i]);
    }
}
