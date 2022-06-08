
#include <math.h>

double fabs(double x) {
    if(x >= 0) {
        return x;
    }
    return -x;
}

float fabsf(float x) {
    if(x >= 0) {
        return x;
    }
    return -x;
}
