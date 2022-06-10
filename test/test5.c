
#include <stdio.h>
#include <math.h>

void test_sqrt(float f) {
    float s = sqrtf(f);
    printf("sqrt of %f is %f\n", f, s);
}

int main() {
    test_sqrt(0.0f);
    test_sqrt(1.0f);
    test_sqrt(2.0f);
    test_sqrt(3.0f);
    test_sqrt(4.0f);
    test_sqrt(7.0f);
    test_sqrt(9.0f);
    test_sqrt(16.0f);
    test_sqrt(256.0f);
    test_sqrt(257.0f);
    return 0;
}
