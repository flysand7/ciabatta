
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    static char str[] = "XYabcZW";
    printf("|1234567890123|\n");
    printf("|%13s|\n", str);
    printf("|%-13.9s|\n", str);
    printf("|%13.10s|\n", str);
    printf("|%13.11s|\n", str);
    printf("|%13.15s|\n", &str[2]);
    printf("|%13c|\n", str[5]);
    return 0;
}
