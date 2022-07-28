#include <assert.h>

int factorial(int n) {
    assert(n >= 0);
    if(n == 0) return 1;
    return n*factorial(n-1);
}

int main() {
    printf("Factorial of %d is %d\n", 10, factorial(10));
    printf("Factorial of %d is %d\n", -1, factorial(-1));
    return 0;
}