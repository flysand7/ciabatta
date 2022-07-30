
#include <stdio.h>

int main(int argc, char **argv) {
    for(int i = 0; i != argc; ++i) {
        char *arg = argv[i];
        printf("% 2d: %s\n", i, arg);
    }
    return 0;
}
