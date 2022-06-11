
#include <math.h>
#include <fenv.h>
#include <stdint.h>
#include <float.h>

#include <_compiler.h>
#if defined(_compiler_clang) || defined(_compiler_gnu)
    #define just_do_it(t) __attribute__((unused)) volatile t
#endif


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


double rint(double x) {
    static const double_t toint = 1/DBL_EPSILON;
    union {double f; uint64_t i;} u = {x};
    int e = u.i>>52 & 0x7ff;
    int s = u.i>>63;
    double y;
    if (e >= 0x3ff+52) return x;
    if (s) y = x - toint + toint;
    else   y = x + toint - toint;
    if (y == 0) return s ? -0.0 : +0.0;
    return y;
}

float rintf(float x) {
    static const float toint = 1/FLT_EPSILON;
    union {float f; uint32_t i;} u = {x};
    int e = u.i>>23 & 0xff;
    int s = u.i>>31;
    float y;
    if (e >= 0x7f+23) return x;
    if (s) y = x - toint + toint;
    else y = x + toint - toint;
    if (y == 0) return s ? -0.0f : 0.0f;
    return y;
}

long double rintl(long double x) {
    return rint(x);
}


double nearbyint(double x) {
    #pragma STDC FENV_ACCESS ON
    int e = fetestexcept(FE_INEXACT);
    x = rint(x);
    if (!e) feclearexcept(FE_INEXACT);
    return x;
}

float nearbyintf(float x) {
    #pragma STDC FENV_ACCESS ON
    int e = fetestexcept(FE_INEXACT);
    x = rintf(x);
    if (!e) feclearexcept(FE_INEXACT);
    return x;
}

long double nearbyintl(long double x) {
    return nearbyint(x);
}


double nextafter(double x, double y) {
    union {double f; uint64_t i;} ux={x}, uy={y};
    uint64_t ax, ay;
    int e;
    if (isnan(x) || isnan(y)) return x + y;
    if (ux.i == uy.i) return y;
    ax = ux.i & -1ULL/2;
    ay = uy.i & -1ULL/2;
    if (ax == 0) {
        if (ay == 0) return y;
        ux.i = (uy.i & 1ULL<<63) | 1;
    } else if (ax > ay || ((ux.i ^ uy.i) & 1ULL<<63)) {
        ux.i--;
    }
    else {
        ux.i++;
    }
    e = ux.i >> 52 & 0x7ff;
    /* raise overflow if ux.f is infinite and x is finite */
    if (e == 0x7ff) just_do_it(float) _x = x+x;
    /* raise underflow if ux.f is subnormal or zero */
    if (e == 0) just_do_it(float) _x = x*x + ux.f*ux.f;
    return ux.f;
}

float nextafterf(float x, float y) {
    union {float f; uint32_t i;} ux={x}, uy={y};
    uint32_t ax, ay, e;

    if (isnan(x) || isnan(y)) return x + y;
    if (ux.i == uy.i) return y;
    ax = ux.i & 0x7fffffff;
    ay = uy.i & 0x7fffffff;
    if (ax == 0) {
        if (ay == 0) return y;
        ux.i = (uy.i & 0x80000000) | 1;
    } else if (ax > ay || ((ux.i ^ uy.i) & 0x80000000)) {
        ux.i--;
    }
    else {
        ux.i++;
    }
    e = ux.i & 0x7f800000;
    /* raise overflow if ux.f is infinite and x is finite */
    if (e == 0x7f800000) just_do_it(float) _x = x+x;
    /* raise underflow if ux.f is subnormal or zero */
    if (e == 0) just_do_it(float) _x = x*x + ux.f*ux.f;
    return ux.f;
}

long double nextafterl(long double x, long double y) {
    return nextafter(x, y);
}

double nexttoward(double x, long double y) {
    return nextafter(x, y);
}

float nexttowardf(float x, long double y) {
    union {float f; uint32_t i;} ux = {x};
    uint32_t e;
    if (isnan(x) || isnan(y)) return x + y;
    if (x == y) return y;
    if (x == 0) {
        ux.i = 1;
        if (signbit(y)) ux.i |= 0x80000000;
    } else if (x < y) {
        if (signbit(x)) ux.i--;
        else            ux.i++;
    } else {
        if (signbit(x)) ux.i++;
        else            ux.i--;
    }
    e = ux.i & 0x7f800000;
    /* raise overflow if ux.f is infinite and x is finite */
    if (e == 0x7f800000) just_do_it(float) _x = x+x;
    /* raise underflow if ux.f is subnormal or zero */
    if (e == 0) just_do_it(float) _x = x*x + ux.f*ux.f;
    return ux.f;
}

long double nexttowardl(long double x, long double y) {
    return nextafterl(x, y);
}


double round(double x) {
    static const double_t toint = 1/DBL_EPSILON;
    union {double f; uint64_t i;} u = {x};
    int e = u.i >> 52 & 0x7ff;
    double_t y;
    if (e >= 0x3ff+52) return x;
    if (u.i >> 63) x = -x;
    if (e < 0x3ff-1) {
        /* raise inexact if x!=0 */
        just_do_it(float) _x = x + toint;
        return 0*u.f;
    }
    y = x + toint - toint - x;
    if (y > 0.5)        y = y + x - 1;
    else if (y <= -0.5) y = y + x + 1;
    else                y = y + x;
    if (u.i >> 63) y = -y;
    return y;
}

float roundf(float x) {
    static const double_t toint = 1/FLT_EPSILON;
    union {float f; uint32_t i;} u = {x};
    int e = u.i >> 23 & 0xff;
    float_t y;
    if (e >= 0x7f+23) return x;
    if (u.i >> 31) x = -x;
    if (e < 0x7f-1) {
        just_do_it(float) _x = x + toint;
        return 0*u.f;
    }
    y = x + toint - toint - x;
    if (y > 0.5f)        y = y + x - 1;
    else if (y <= -0.5f) y = y + x + 1;
    else                 y = y + x;
    if (u.i >> 31) y = -y;
    return y;
}

long double roundl(long double x) {
    return round(x);
}
