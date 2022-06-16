
#include <math.h>
#include <fenv.h>
#include <stdint.h>
#include <float.h>

int _fpclassify(double x) {
    union {double f; uint64_t i;} u = {x};
    int e = u.i>>52 & 0x7ff;
    if (!e) return u.i<<1 ? FP_SUBNORMAL : FP_ZERO;
    if (e==0x7ff) return u.i<<12 ? FP_NAN : FP_INFINITE;
    return FP_NORMAL;
}

int _fpclassifyf(float x) {
    union {float f; uint32_t i;} u = {x};
    int e = u.i>>23 & 0xff;
    if (!e) return u.i<<1 ? FP_SUBNORMAL : FP_ZERO;
    if (e==0xff) return u.i<<9 ? FP_NAN : FP_INFINITE;
    return FP_NORMAL;
}

int _fpclassifyl(long double x) {
    return _fpclassify(x);
}


int _signbit(double x) {
    union {
        double d;
        uint64_t i;
    } y = { x };
    return y.i>>63;
}

int _signbitf(float x) {
    union {
        float f;
        uint32_t i;
    } y = { x };
    return y.i>>31;
}

int _signbitl(long double x) {
    return _signbit(x);
}

double copysign(double x, double y) {
    union {double f; uint64_t i;} ux={x}, uy={y};
    ux.i &= ~(1ULL<<63);
    ux.i |= uy.i & (1ULL<<63);
    return ux.f;
}

float copysignf(float x, float y) {
    union {float f; uint32_t i;} ux={x}, uy={y};
    ux.i &= 0x7fffffff;
    ux.i |= uy.i & 0x80000000;
    return ux.f;
}

long double copysignl(long double x, long double y) {
    return copysign(x, y);
}

double nan(const char *s) {
    return NAN;
}

float nanf(const char *s) {
    return NAN;
}

long double nanl(const char *s) {
    return NAN;
}

