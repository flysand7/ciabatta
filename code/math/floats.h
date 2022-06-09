
#include <math.h>

#define f_man_bits (f_sig_bits - 1)
#define f_exp_bits (f_bits - f_man_bits - 1)

#define b_sign_val(b) ((b) >> (f_exp_bits + f_man_bits))
#define b_bexp_val(b) (((b) >> f_man_bits) & f_exp_mask)
#define b_mant_val(b) ((b) & f_man_mask)
#define b_mant_mask   ((1 << f_man_bits) - 1)
#define b_bexp_mask   ((1 << f_exp_bits) - 1)
#define b_mant_max    ((1 << f_man_bits) - 1)
#define b_bexp_max    ((1 << f_exp_bits) - 1)

#define b_exp_bias    ((1 << (f_exp_bits-1)) - 1)
#define b_exp_val(b)  (b_bexp_val(b) - b_exp_bias)

static inline itype suffix(getbits)(ftype f) {
    union _f {
        ftype f;
        itype i;
    } u;
    u.f = f;
    int bits = u.i;
    return bits;
}

int suffix(_fpclassify)(ftype f) {
    itype bits = suffix(getbits)(f);
    itype sign = b_sign_val(bits);
    itype bexp = b_bexp_val(bits);
    itype mant = b_mant_val(bits);
    if(bexp == b_bexp_max) {
        if(mant == 0) return FP_INFINITE;
        else          return FP_NAN;
    }
    else if(bexp == 0) {
        if(mant == 0) return FP_ZERO;
        else          return FP_SUBNORMAL;
    }
    else {
        return FP_NORMAL;
    }
}

