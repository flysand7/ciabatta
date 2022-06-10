
#include <errno.h>

#if !defined(_isqrt_defined)
#define _isqrt_defined
static uint64_t _isqrt(uint64_t num, uint64_t *remp) {
    // To find a square root of a number
    // We get rid of zero
    if(num == 0) {
        *remp = 0;
        return 0;
    }
    // Then, starting from the bottom, split num into 2-digit pairs
    // and find the top-most non-zero pair
    uint64_t i = 0;
    while(i != (sizeof(uint64_t)*8) && (num >> i) != 0) {
        i += 2;
    }
    // Then we start taking guesses such that at each step
    // sqrt^2 <= number made of consequent pairs of exausted integers
    uint64_t sqrt = 0;
    uint64_t rem = 0;
    // Repeat until remainder is equal to zero:
    do {
        i -= 2;
        // Bring the next two digits of the number to our remainder
        rem = (rem << 2) | ((num >> i) & 0x3);
        // Find d such that d(2sqrt+d) <= rem
        // Since d could be either 0 or 1 we simply check 1, otherwise its 0
        uint64_t d = 1;
        uint64_t t = ((sqrt<<2)|1);
        if(t <= rem) {
            rem -= t;
        }
        else {
            d = 0;
        }
        // Append the digit to sqrt from the right
        sqrt = (sqrt<<1)|d;
    } while(i != 0);

    *remp = rem;
    return sqrt;
}
#endif

// For all it's worth this shit is simply equivalent to
// _isqrt((uint64)x)
// I hate porgaming.
ftype suffix(sqrt)(ftype x) {
    if(x < 0) {
        #if math_errhandling & MATH_ERRNO
            errno = EDOM;
        #endif
        return NAN;
    }
    if(x == 0 || isinf(x)) {
        return x;
    }
    if(isnan(x)) {
        return NAN;
    }
    itype bits = suffix(f_bits)(x);
    itype exp = f_qexp(bits);
    itype man = f_qman(bits);
    // Get lots of precision by shifting man right by max bits
    // and subtracting this from the exponent
    itype bit = 0; // highest set-bit of man
    while((man >> (bit+1)) != 0) ++bit;
    itype prec_shift_n = f_nbits - bit - 3;
    man <<= prec_shift_n;
    exp -= prec_shift_n;
    // Now do the sqrt of 2^exp * man
    // If exp is odd then 2^{2k+1}*sqrt(man) = 2^{2k}*sqrt{2*man}
    if((2 + (exp % 2)) % 2 != 0) {
        man <<= 1;
    }
    // Take exp sqrt
    exp >>= 1;
    // Take sqrt of mantissa
    uint64_t rem;
    man = (itype)_isqrt(man, &rem);
    // Now sqrt(x) = 2^exp * man
    // we need to normalize this shit
    bit = 0; // highest set-bit of man
    while((man >> (bit+1)) != 0) ++bit;
    exp += bit;
    man <<= f_nbits-bit;
    exp += f_ebias;
    man >>= f_nbits-f_mbits;
    man &= f_mmask;
    // Cons it back
    bits = b_cons(0, exp, man);
    return suffix(f_frombits)(bits);
}
