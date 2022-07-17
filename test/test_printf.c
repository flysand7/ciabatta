
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    int w = printf("%- 16.8da\n", INT_MIN);
    printf("%d\n", w);
    return 0;
}
