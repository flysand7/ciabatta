
#define asuint64(x) ((union {f64 f; uint64_t i;}){x}).i
#define asdouble(x) ((union {f64 f; uint64_t i;}){x}).f

#if defined(__GNUC__) || defined(__clang__)
    #define just_do_it(v) do{__attribute__((unused)) volatile f64 t = v;}while(0)
#else
    #define just_do_it(v) do{volatile f64 t = v;}while(0)
#endif

f64 nearbyint(f64 x) {
    #pragma STDC FENV_ACCESS ON
    u64 bits = F64_BITS(x);
    i64 bexp = F64_BEXP(bits);
    u64 bmant = F64_MANT(bits);
    // 1. Get rid of special cases, exp = 0x7ff, and exp < 0x3ff
    // Return x unmodified if inf, nan
    if(bexp == 0x7ff) {
        return x;
    }
    int mode = fegetround();
    // Get exponent for (integer_mantissa * 2^exp) representation
    i64 exp = bexp - 0x3ff - 52;
    int s = F64_SIGN(bits);
    // This value is 0 if no increment is required, and 1 if the absolute value
    // increases by 1
    int c;
    {
        // Check if we need to round towards 0 or towards 1
        // (assumes specific values in rounding modes in fenv.h)
        int a = (mode&2)>>1;
        int b = mode&1;
        int mask = ((a^b)<<1)|(a^b);
        int d = 2 - mode&mask;
        c = s ^ d;
    }
    // If the whole mantissa is after a point, such that the first digit is 0,
    // then the value is closer to 0 these values are all zeroes, subnormal
    // numbers and very small normal numbers
    if(exp < -53) {
        // Return 0 if exponent and mantissa are zero
        if(bexp == 0 && bmant == 0) {
            return x;
        }
        // For subnormal and normal numbers we round them either towards 0 or 1
        // and then call it a day
        u64 new_bexp = (u64)((1-c)&0x3ff) << F64_MANT_BITS;
        u64 new_sign = (u64)s << 63;
        u64 new_bits = new_sign | new_bexp;
        return F64_CONS(new_bits);
    }
    // 2. Get fractional and whole bits of the mantissa
    u64 mant = bmant | ((u64)1 << 52);
    if(exp >= 0) {
        // Already an integer
        return x;
    }
    // if e.g. mantissa is 0b101.., and exponent is -2, the value is 0b101*2^-2
    // or 0b1.01, meaning there are 2 fractional digits
    int nfrac_digs = -exp;
    u64 frac_mask = (((u64)1<<(nfrac_digs))-1);
    u64 frac_mant = mant & frac_mask;
    // The mantissas for 1.0 and 0.5
    u64 one = (((u64)1<<(nfrac_digs)));
    u64 half = one >> 1;
    // 3. Round the float based on the value of c
    // we'll first fix up c to include other rounding modes
    c |= (mode == FE_UPWARD)    & ((~s)&1);
    c |= (mode == FE_DOWNWARD)  & s;
    c |= (mode == FE_TONEAREST) & (frac_mant >= half);
    // Drop fractional bits
    u64 new_mant = mant & ~frac_mant;
    // Add 1 to float if required
    if(c) {
        new_mant += one;
        if(new_mant > ((u64)1 << 53)) {
            new_mant >>= 1;
            exp += 1;
        }
    }
    new_mant &= F64_MANT_MASK;
    u64 new_bits = new_mant;
    new_bits |= (exp+0x3ff+52) << F64_MANT_BITS;
    new_bits |= (u64)s << (F64_MANT_BITS + F64_BEXP_BITS);
    f64 result = F64_CONS(new_bits);
    return result;
}

f32 nearbyintf(f32 x) {
    #pragma STDC FENV_ACCESS ON
    u64 bits = F32_BITS(x);
    i64 bexp = F32_BEXP(bits);
    u64 bmant = F32_MANT(bits);
    if(bexp == 0x7f) {
        return x;
    }
    int mode = fegetround();
    i64 exp = bexp - 0x3f - 52;
    int s = F32_SIGN(bits);
    int c;
    {
        int a = (mode&2)>>1;
        int b = mode&1;
        int mask = ((a^b)<<1)|(a^b);
        int d = 2 - mode&mask;
        c = s ^ d;
    }
    if(exp < -24) {
        if(bexp == 0 && bmant == 0) {
            return x;
        }
        u64 new_bexp = (u64)((1-c)&0x3f) << F32_MANT_BITS;
        u64 new_sign = (u64)s << 63;
        u64 new_bits = new_sign | new_bexp;
        return F32_CONS(new_bits);
    }
    u64 mant = bmant | ((u64)1 << 23);
    if(exp >= 0) {
        return x;
    }
    int nfrac_digs = -exp;
    u64 frac_mask = (((u64)1<<(nfrac_digs))-1);
    u64 frac_mant = mant & frac_mask;
    u64 one = (((u64)1<<(nfrac_digs)));
    u64 half = one >> 1;
    c |= (mode == FE_UPWARD)    & ((~s)&1);
    c |= (mode == FE_DOWNWARD)  & s;
    c |= (mode == FE_TONEAREST) & (frac_mant >= half);
    u64 new_mant = mant & ~frac_mant;
    if(c) {
        new_mant += one;
        if(new_mant > ((u64)1 << 24)) {
            new_mant >>= 1;
            exp += 1;
        }
    }
    new_mant &= F32_MANT_MASK;
    u64 new_bits = new_mant;
    new_bits |= (exp+0x3f+23) << F32_MANT_BITS;
    new_bits |= (u64)s << (F32_MANT_BITS + F32_BEXP_BITS);
    f64 result = F32_CONS(new_bits);
    return result;
}

fl64 nearbyintl(fl64 x) {
    return nearbyint((f64)x);
}

f64 nextafter(f64 x, f64 y) {
    union {f64 f; uint64_t i;} ux={x}, uy={y};
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
    e = ux.i >> 52 & 0x7f;
    /* raise overflow if ux.f is infinite and x is finite */
    if (e == 0x7f) just_do_it(x+x);
    /* raise underflow if ux.f is subnormal or zero */
    if (e == 0) just_do_it(x*x + ux.f*ux.f);
    return ux.f;
}

f32 nextafterf(f32 x, f32 y) {
    union {f32 f; uint32_t i;} ux={x}, uy={y};
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

fl64 nextafterl(fl64 x, fl64 y) {
    return nextafter(x, y);
}

f64 nexttoward(f64 x, fl64 y) {
    return nextafter(x, y);
}

f32 nexttowardf(f32 x, fl64 y) {
    union {f32 f; uint32_t i;} ux = {x};
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

fl64 nexttowardl(fl64 x, fl64 y) {
    return nextafterl(x, y);
}

f64 rint(f64 x) {
    static const double_t toint = 1/DBL_EPSILON;
    union {f64 f; uint64_t i;} u = {x};
    int e = u.i>>52 & 0x7ff;
    int s = u.i>>63;
    f64 y;
    if (e >= 0x3ff+52) return x;
    if (s) y = x - toint + toint;
    else   y = x + toint - toint;
    if (y == 0) return s ? -0.0 : +0.0;
    return y;
}

f32 rintf(f32 x) {
    static const f32 toint = 1/FLT_EPSILON;
    union {f32 f; uint32_t i;} u = {x};
    int e = u.i>>23 & 0xff;
    int s = u.i>>31;
    f32 y;
    if (e >= 0x7f+23) return x;
    if (s) y = x - toint + toint;
    else y = x + toint - toint;
    if (y == 0) return s ? -0.0f : 0.0f;
    return y;
}

fl64 rintl(fl64 x) {
    return rint(x);
}

#if LONG_MAX < 1U<<53 && defined(FE_INEXACT)
    static long lrint_slow(f64 x)
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

    long lrint(f64 x)
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
    long lrint(f64 x) {
        return rint(x);
    }
#endif

long lrintf(f32 x) {
    return rintf(x);
}

long lrintl(fl64 x) {
    return lrint(x);
}

long long llrint(f64 x) {
    return rint(x);
}

long long llrintf(f32 x) {
    return rintf(x);
}

long long llrintl(fl64 x) {
    return llrint(x);
}

f64 round(f64 x) {
    static const double_t toint = 1/DBL_EPSILON;
    union {f64 f; uint64_t i;} u = {x};
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

f32 roundf(f32 x) {
    static const double_t toint = 1/FLT_EPSILON;
    union {f32 f; uint32_t i;} u = {x};
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

fl64 roundl(fl64 x) {
    return round(x);
}

long lround(f64 x) {
    return round(x);
}

long lroundf(f32 x) {
    return roundf(x);
}

long lroundl(fl64 x) {
    return roundl(x);
}

long long llround(f64 x) {
    return round(x);
}

long long llroundf(f32 x) {
    return roundf(x);
}

long long llroundl(fl64 x) {
    return roundl(x);
}

f64 ceil(f64 x) {
    static const double_t toint = 1/DBL_EPSILON;
    union {f64 f; uint64_t i;} u = {x};
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

f32 ceilf(f32 x) {
    union {f32 f; uint32_t i;} u = {x};
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

fl64 ceill(fl64 x) {
    return ceil(x);
}

f64 floor(f64 x) {
    static const double_t toint = 1/DBL_EPSILON;
    union {f64 f; uint64_t i;} u = {x};
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

f32 floorf(f32 x) {
    union {f32 f; uint32_t i;} u = {x};
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

fl64 floorl(fl64 x) {
    return floor(x);
}

f64 trunc(f64 x) {
    union {f64 f; uint64_t i;} u = {x};
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

f32 truncf(f32 x) {
    union {f32 f; uint32_t i;} u = {x};
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

fl64 truncl(fl64 x) {
    return trunc(x);
}
