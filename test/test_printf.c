
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    printf("%a\n", 0.0);
    printf("%a\n", 2.0);
    printf("%#a\n", 128.0);
    printf("%a\n", 128.0);
    printf("%16a|\n", 128.0);
    printf("%16.2a|\n", 128.0);
    printf("%-16.2a|\n", 128.0);
    printf("%016.2a|\n", 128.0);
    printf("%.2A\n", 3.141592);
    return 0;
}
