
// This thing doesn't compile branchlessly on MSVC lmao
// This is a branchless version of fpclassify
// The way it works is by assuming the following structure of float class
// bit 2 | bit 1 | bit 0
//   e       n      m
// e is 1 for infinities and nan, 0 otherwise
// n is 1 for normal numbers, 0 otherwise
// m is 1 for nan and subnormal numbers, 0 otherwise
// This leaves the following values for classes:
//  0 - FP_ZERO
//  1 - FP_SUBNORMAL
//  2 - FP_NORMAL
//  4 - FP_INFINITE
//  5 - FP_NAN
int _fpclassify(f64 x) {
    // First, extract bits
    u64 bits = F64_BITS(x);
    i64 exp  = F64_BEXP(bits);
    i64 mant = F64_MANT(bits);
    // Get the number that's only zero when exp = 0x7ff
    i64 ee = F64_BEXP_MAX - exp;
    // Invert it to get number that's 1 iff x is infinity or nan
    i64 e = !(ee);
    // The value (ee*exp) is zero in two cases:
    //   exp = 0x7ff, (infinities, nan)
    //   exp = 0 (zero, subnormal)
    // We negate this to make it so that this value is 1 only if number is not
    // normal
    i64 nn = !(ee*exp);
    // Negate the previous thing. Now n is 1 iff number is normal
    i64 n = !nn;
    // m is 1 if mantissa is nonzero and the number is not normal
    i64 m = !!mant & nn;
    // Construct the float class
    return (e<<2) | (n<<1) | m;
}

// Same recipe as above, different constants
int _fpclassifyf(float x) {
    u64 bits = F32_BITS(x);
    i64 exp  = F32_BEXP(bits);
    i64 mant = F32_MANT(bits);
    i64 ee = F32_BEXP_MAX - exp;
    i64 e = !(ee);
    i64 nn = !(ee*exp);
    i64 n = !nn;
    i64 m = !!mant & nn;
    return (e<<2) | (n<<1) | m;
}

int _fpclassifyl(fl64 x) {
    return _fpclassify((f64)x);
}


int _signbit(f64 x) {
    union {
        f64 d;
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

int _signbitl(fl64 x) {
    return _signbit(x);
}

f64 copysign(f64 x, f64 y) {
    union {f64 f; uint64_t i;} ux={x}, uy={y};
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

fl64 copysignl(fl64 x, fl64 y) {
    return copysign(x, y);
}

f64 nan(const char *s) {
    return NAN;
}

float nanf(const char *s) {
    return NAN;
}

fl64 nanl(const char *s) {
    return NAN;
}

