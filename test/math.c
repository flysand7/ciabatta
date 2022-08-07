
#include <std.h>

double cool_doubles[] = {
    -NAN,
    -INFINITY,
    -DBL_MAX,
    -2.7,
    -2.5,
    -2.3,
    -1.0,
    -DBL_TRUE_MIN,
    -DBL_MIN,
    -0.0,
    +0.0,
    DBL_MIN,
    DBL_TRUE_MIN,
    1.0,
    2.3,
    2.5,
    2.7,
    DBL_MAX,
    INFINITY,
    NAN,
};

size_t ncool_doubles = COUNTOF(cool_doubles);

#define for_each_cool_double(i,d,b)           \
    for(int i = 0; i != ncool_doubles; ++i) { \
        double d = cool_doubles[i];           \
        b;                                    \
    }

const char *show_classification(double x) {
    switch(fpclassify(x)) {
        case FP_INFINITE:  return "Inf";
        case FP_NAN:       return "NaN";
        case FP_NORMAL:    return "normal";
        case FP_SUBNORMAL: return "subnormal";
        case FP_ZERO:      return "zero";
        default:           return "unknown";
    }
}

int main() {
    for(int i = 0; i != ncool_doubles; ++i) {
        double d = cool_doubles[i];
        printf("%a\n", d);
    }
    return 0;
}
