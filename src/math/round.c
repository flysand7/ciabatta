
#define asuint64(x) ((union {double f; uint64_t i;}){x}).i
#define asdouble(x) ((union {double f; uint64_t i;}){x}).f

#if defined(__GNUC__) || defined(__clang__)
    #define just_do_it(v) do{__attribute__((unused)) volatile double t = v;}while(0)
#else
    #define just_do_it(v) do{volatile double t = v;}while(0)
#endif

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
    if (e == 0x7ff) just_do_it(x+x);
    /* raise underflow if ux.f is subnormal or zero */
    if (e == 0) just_do_it(x*x + ux.f*ux.f);
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
    if (e == 0x7f800000) just_do_it(x+x);
    /* raise underflow if ux.f is subnormal or zero */
    if (e == 0) just_do_it(x*x + ux.f*ux.f);
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
    if (e == 0x7f800000) just_do_it(x+x);
    /* raise underflow if ux.f is subnormal or zero */
    if (e == 0) just_do_it(x*x + ux.f*ux.f);
    return ux.f;
}

long double nexttowardl(long double x, long double y) {
    return nextafterl(x, y);
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

#if LONG_MAX < 1U<<53 && defined(FE_INEXACT)
    static long lrint_slow(double x)
    {
        #pragma STDC FENV_ACCESS ON
        int e;
        e = fetestexcept(FE_INEXACT);
        x = rint(x);
        if (!e && (x > LONG_MAX || x < LONG_MIN))
            feclearexcept(FE_INEXACT);
        /* conversion */
        return x;
    }

    long lrint(double x)
    {
        uint32_t abstop = asuint64(x)>>32 & 0x7fffffff;
        uint64_t sign = asuint64(x) & (1ULL << 63);

        if (abstop < 0x41dfffff) {
            /* |x| < 0x7ffffc00, no overflow */
            double_t toint = asdouble(asuint64(1/DBL_EPSILON) | sign);
            double_t y = x + toint - toint;
            return (long)y;
        }
        return lrint_slow(x);
    }
#else
    long lrint(double x) {
        return rint(x);
    }
#endif

long lrintf(float x) {
    return rintf(x);
}

long lrintl(long double x) {
    return lrint(x);
}

long long llrint(double x) {
    return rint(x);
}

long long llrintf(float x) {
    return rintf(x);
}

long long llrintl(long double x) {
    return llrint(x);
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
        just_do_it(x + toint);
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
        just_do_it(x + toint);
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

long lround(double x) {
    return round(x);
}

long lroundf(float x) {
    return roundf(x);
}

long lroundl(long double x) {
    return roundl(x);
}

long long llround(double x) {
    return round(x);
}

long long llroundf(float x) {
    return roundf(x);
}

long long llroundl(long double x) {
    return roundl(x);
}

double ceil(double x) {
    static const double_t toint = 1/DBL_EPSILON;
    union {double f; uint64_t i;} u = {x};
    int e = u.i >> 52 & 0x7ff;
    double_t y;

    if (e >= 0x3ff+52 || x == 0)
        return x;
    /* y = int(x) - x, where int(x) is an integer neighbor of x */
    if (u.i >> 63)
        y = x - toint + toint - x;
    else
        y = x + toint - toint - x;
    /* special case because of non-nearest rounding modes */
    if (e <= 0x3ff-1) {
        just_do_it(y);
        return u.i >> 63 ? -0.0 : 1;
    }
    if (y < 0)
        return x + y + 1;
    return x + y;
}

float ceilf(float x) {
    union {float f; uint32_t i;} u = {x};
    int e = (int)(u.i >> 23 & 0xff) - 0x7f;
    uint32_t m;

    if (e >= 23)
        return x;
    if (e >= 0) {
        m = 0x007fffff >> e;
        if ((u.i & m) == 0)
            return x;
        just_do_it(x + 0x1p120f);
        if (u.i >> 31 == 0)
            u.i += m;
        u.i &= ~m;
    } else {
        just_do_it(x + 0x1p120f);
        if (u.i >> 31)
            u.f = -0.0;
        else if (u.i << 1)
            u.f = 1.0;
    }
    return u.f;
}

long double ceill(long double x) {
    return ceil(x);
}

double floor(double x) {
    static const double_t toint = 1/DBL_EPSILON;
    union {double f; uint64_t i;} u = {x};
    int e = u.i >> 52 & 0x7ff;
    double_t y;
    if (e >= 0x3ff+52 || x == 0)
        return x;
    /* y = int(x) - x, where int(x) is an integer neighbor of x */
    if (u.i >> 63)
        y = x - toint + toint - x;
    else
        y = x + toint - toint - x;
    /* special case because of non-nearest rounding modes */
    if (e <= 0x3ff-1) {
        just_do_it(y);
        return u.i >> 63 ? -1 : 0;
    }
    if (y > 0)
        return x + y - 1;
    return x + y;
}

float floorf(float x) {
    union {float f; uint32_t i;} u = {x};
    int e = (int)(u.i >> 23 & 0xff) - 0x7f;
    uint32_t m;

    if (e >= 23)
        return x;
    if (e >= 0) {
        m = 0x007fffff >> e;
        if ((u.i & m) == 0)
            return x;
        just_do_it(x + 0x1p120f);
        if (u.i >> 31)
            u.i += m;
        u.i &= ~m;
    } else {
        just_do_it(x + 0x1p120f);
        if (u.i >> 31 == 0)
            u.i = 0;
        else if (u.i << 1)
            u.f = -1.0;
    }
    return u.f;
}

long double floorl(long double x) {
    return floor(x);
}

double trunc(double x) {
    union {double f; uint64_t i;} u = {x};
    int e = (int)(u.i >> 52 & 0x7ff) - 0x3ff + 12;
    uint64_t m;

    if (e >= 52 + 12)
        return x;
    if (e < 12)
        e = 1;
    m = -1ULL >> e;
    if ((u.i & m) == 0)
        return x;
    just_do_it(x + 0x1p120f);
    u.i &= ~m;
    return u.f;
}

float truncf(float x) {
    union {float f; uint32_t i;} u = {x};
    int e = (int)(u.i >> 23 & 0xff) - 0x7f + 9;
    uint32_t m;

    if (e >= 23 + 9)
        return x;
    if (e < 9)
        e = 1;
    m = -1U >> e;
    if ((u.i & m) == 0)
        return x;
    just_do_it(x + 0x1p120f);
    u.i &= ~m;
    return u.f;
}

long double truncl(long double x) {
    return trunc(x);
}
