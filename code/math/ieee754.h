
// Instantiates 'template' macros for floating-point values
// When including expecting the following parameters to be defined:
//  ftype: Floating-point type to consider
//  itype: Signed integer type corresponding to ftype's bitwidth
//  f_ebits: Number of bits in the exponent
//  f_mbits: Number of bits in the mantissa
//  suffix(name): appends corresponding suffix to the given name,
//                e.g. f for floats

#define f_nbits (1+f_ebits+f_mbits)
#define f_emax ((1ULL << (f_ebits-1)) - 1)
#define f_emin (1 - f_emax)

// Extracting fields from the float
#define f_eoffs (f_mbits)
#define f_soffs (f_mbits+f_ebits)
#define f_emask ((1ULL << f_ebits) - 1)
#define f_mmask ((1ULL << f_mbits) - 1)
#define f_smask 1ULL
#define f_eval(b) (((b) >> f_eoffs) & f_emask)
#define f_sval(b) (((b) >> f_soffs) & f_smask)
#define f_mval(b) (((b) >> 0) & f_mmask)
#define f_abs(b)  ((b) & ~(f_smask << f_soffs))

#define b_cons(s,e,m) ((s << f_soffs) | (e << f_eoffs) | (m))

// Converting float to integer bits
static inline itype suffix(f_bits)(ftype f) {
    union _u {
        ftype f;
        itype b;
    } u;
    u.f = f;
    return u.b;
}

static inline itype suffix(f_frombits)(itype b) {
        union _u {
        ftype f;
        itype b;
    } u;
    u.b = b;
    return u.f;
}

// Floating-point classification
int suffix(_fpclassify)(ftype f) {
    itype bits = suffix(f_bits)(f);
    itype exp = f_eval(bits);
    itype man = f_mval(bits);
    if(exp == f_emax) {
        if(man == 0) return FP_INFINITE;
        else return FP_NAN;
    }
    else if(exp == 0) {
        if(man == 0) return FP_ZERO;
        else return FP_SUBNORMAL;
    }
    else return FP_NORMAL;
}

int suffix(_signbit)(ftype f) {
    itype bits = suffix(f_bits)(f);
    itype sign = f_sval(bits);
    return sign;
}

ftype suffix(copysign)(ftype x, ftype y) {
    itype xbits = suffix(f_bits)(x);
    itype ybits = suffix(f_bits)(y);
    itype exp = f_eval(xbits);
    itype man = f_mval(xbits);
    itype sgn = f_sval(ybits);
    itype rbits = b_cons(sgn, exp, man);
    return suffix(f_frombits)(rbits);
}

// Floating-point non-signaling comparison

static Ordering suffix(_ordering)(ftype x, ftype y) {
    itype xclass = suffix(_fpclassify)(x);
    itype yclass = suffix(_fpclassify)(y);
    if(xclass == FP_NAN || yclass == FP_NAN) {
        return UN;
    }
    itype xbits = suffix(f_bits)(x);
    itype ybits = suffix(f_bits)(y);
    itype xsgn = f_sval(xbits);
    itype ysgn = f_sval(ybits);
    itype xabs = f_abs(xbits);
    itype yabs = f_abs(ybits);
    if(xsgn == ysgn) {
        if(xabs > yabs) return GR;
        if(xabs < yabs) return LS;
        return EQ;
    }
    else {
        if(xabs == 0 && yabs == 0) return EQ;
        if(xsgn) return LS;
        if(ysgn) return GR;
    }
    return UN; // I may be stupid
}

int suffix(_isgrt)(ftype x, ftype y) {
    int ord = suffix(_ordering)(x, y);
    return ord == GR;
}

int suffix(_isgeq)(ftype x, ftype y) {
    int ord = suffix(_ordering)(x, y);
    return ord == GR || ord == EQ;
}

int suffix(_isles)(ftype x, ftype y) {
    int ord = suffix(_ordering)(x, y);
    return ord == LS;
}

int suffix(_isleq)(ftype x, ftype y) {
    int ord = suffix(_ordering)(x, y);
    return ord == LS || ord == EQ;
}

int suffix(_isleg)(ftype x, ftype y) {
    int ord = suffix(_ordering)(x, y);
    return ord == LS || ord == GR;
}

int suffix(_isuno)(ftype x, ftype y) {
    int ord = suffix(_ordering)(x, y);
    return ord == UN;
}