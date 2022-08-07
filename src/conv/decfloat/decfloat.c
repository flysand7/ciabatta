
#include "decfloat_table.h"

typedef struct decfloat_t decfloat_t;
struct decfloat_t {
    u64 sign;
    u64 exponent;
    i64 mantissa;
};

#define DOUBLE_MANTISSA_BITS 52
#define DOUBLE_EXPONENT_BITS 11
#define DOUBLE_BIAS 1023

static const char DIGIT_TABLE[200] = {
    "00010203040506070809101112131415161718192021222324"
    "25262728293031323334353637383940414243444546474849"
    "50515253545556575859606162636465666768697071727374"
    "75767778798081828384858687888990919293949596979899"
};

static inline u32 pow5Factor(u64 value) {
  const u64 m_inv_5 = 14757395258967641293u; // 5 * m_inv_5 = 1 (mod 2^64)
  const u64 n_div_5 = 3689348814741910323u;  // #{ n | n = 0 (mod 2^64) } = 2^64 / 5
  u32 count = 0;
  for (;;) {
      value *= m_inv_5;
      if (value > n_div_5)
          break;
      ++count;
  }
  return count;
}

// Returns true if value is divisible by 5^p.
static inline bool multipleOfPowerOf5(const u64 value, const u32 p) {
    // I tried a case distinction on p, but there was no performance difference.
    return pow5Factor(value) >= p;
}

// Returns true if value is divisible by 2^p.
static inline bool multipleOfPowerOf2(const u64 value, const u32 p) {
    // __builtin_ctzll doesn't appear to be faster here.
    return (value & ((1ull << p) - 1)) == 0;
}

static inline u64 umul128(const u64 a, const u64 b, u64* const productHi) {
    return _umul128(a, b, productHi);
}

static inline u64 umulh(const u64 a, const u64 b) {
    u64 hi;
    umul128(a, b, &hi);
    return hi;
}

static inline u64 shiftright128(const u64 lo, const u64 hi, const u32 dist) {
    return __shiftright128(lo, hi, (unsigned char) dist);
}

static inline u64 mulShift64(const u64 m, const u64* const mul, const int32_t j) {
                // m is maximum 55 bits
    u64 high1;                                   // 128
    const u64 low1 = umul128(m, mul[1], &high1); // 64
    u64 high0;                                   // 64
    umul128(m, mul[0], &high0);                       // 0
    const u64 sum = high0 + low1;
    if (sum < high0) {
        ++high1; // overflow into high1
    }
    return shiftright128(sum, high1, j - 64);
}


static inline u64 mulShiftAll64(const u64 m, const u64* const mul, const int32_t j,
    u64* const vp, u64* const vm, const u32 mmShift) {
    *vp = mulShift64(4 * m + 2, mul, j);
    *vm = mulShift64(4 * m - 1 - mmShift, mul, j);
    return mulShift64(4 * m, mul, j);
}

// Returns floor(log_10(2^e)); requires 0 <= e <= 1650.
static inline u32 log10Pow2(const int32_t e) {
    // The first value this approximation fails for is 2^1651 which is just greater than 10^297.
    return (((u32) e) * 78913) >> 18;
}

// Returns floor(log_10(5^e)); requires 0 <= e <= 2620.
static inline u32 log10Pow5(const int32_t e) {
    // The first value this approximation fails for is 5^2621 which is just greater than 10^1832.
    return (((u32) e) * 732923) >> 20;
}

// Returns e == 0 ? 1 : ceil(log_2(5^e)); requires 0 <= e <= 3528.
static inline int32_t pow5bits(const int32_t e) {
    // This approximation works up to the point that the multiplication overflows at e = 3529.
    // If the multiplication were done in 64 bits, it would fail at 5^4004 which is just greater
    // than 2^9297.
    return (int32_t) (((((u32) e) * 1217359) >> 19) + 1);
}

static inline u32 decimalLength17(const u64 v) {
    // This is slightly faster than a loop.
    // The average output length is 16.38 digits, so we check high-to-low.
    // Function precondition: v is not an 18, 19, or 20-digit number.
    // (17 digits are sufficient for round-tripping.)
    if (v >= 10000000000000000L) { return 17; }
    if (v >= 1000000000000000L) { return 16; }
    if (v >= 100000000000000L) { return 15; }
    if (v >= 10000000000000L) { return 14; }
    if (v >= 1000000000000L) { return 13; }
    if (v >= 100000000000L) { return 12; }
    if (v >= 10000000000L) { return 11; }
    if (v >= 1000000000L) { return 10; }
    if (v >= 100000000L) { return 9; }
    if (v >= 10000000L) { return 8; }
    if (v >= 1000000L) { return 7; }
    if (v >= 100000L) { return 6; }
    if (v >= 10000L) { return 5; }
    if (v >= 1000L) { return 4; }
    if (v >= 100L) { return 3; }
    if (v >= 10L) { return 2; }
    return 1;
}

static inline u64 div5(const u64 x) {
    return umulh(x, 0xCCCCCCCCCCCCCCCDu) >> 2;
}

static inline u64 div10(const u64 x) {
    return umulh(x, 0xCCCCCCCCCCCCCCCDu) >> 3;
}

static inline u64 div100(const u64 x) {
    return umulh(x >> 2, 0x28F5C28F5C28F5C3u) >> 2;
}

static inline u64 div1e8(const u64 x) {
    return umulh(x, 0xABCC77118461CEFDu) >> 26;
}

static decfloat_t ieee_to_decimal(u64 sign, u64 ieeeMantissa, u32 ieeeExponent) {
    int32_t e2;
    u64 m2;
    if (ieeeExponent == 0) {
                // We subtract 2 so that the bounds computation has 2 additional bits.
        e2 = 1 - DOUBLE_BIAS - DOUBLE_MANTISSA_BITS - 2;
        m2 = ieeeMantissa;
    } else {
        e2 = (int32_t) ieeeExponent - DOUBLE_BIAS - DOUBLE_MANTISSA_BITS - 2;
        m2 = (1ull << DOUBLE_MANTISSA_BITS) | ieeeMantissa;
    }
    const bool even = (m2 & 1) == 0;
    const bool acceptBounds = even;

    // Step 2: Determine the interval of valid decimal representations.
    const u64 mv = 4 * m2;
    // Implicit bool -> int conversion. True is 1, false is 0.
    const u32 mmShift = ieeeMantissa != 0 || ieeeExponent <= 1;
    // We would compute mp and mm like this:
    // u64 mp = 4 * m2 + 2;
    // u64 mm = mv - 1 - mmShift;

    // Step 3: Convert to a decimal power base using 128-bit arithmetic.
    u64 vr, vp, vm;
    int32_t e10;
    bool vmIsTrailingZeros = false;
    bool vrIsTrailingZeros = false;
    if (e2 >= 0) {
        // I tried special-casing q == 0, but there was no effect on performance.
        // This expression is slightly faster than max(0, log10Pow2(e2) - 1).
        const u32 q = log10Pow2(e2) - (e2 > 3);
        e10 = (int32_t) q;
        const int32_t k = DOUBLE_POW5_INV_BITCOUNT + pow5bits((int32_t) q) - 1;
        const int32_t i = -e2 + (int32_t) q + k;
        vr = mulShiftAll64(m2, DOUBLE_POW5_INV_SPLIT[q], i, &vp, &vm, mmShift);
        if (q <= 21) {
            // This should use q <= 22, but I think 21 is also safe. Smaller values
            // may still be safe, but it's more difficult to reason about them.
            // Only one of mp, mv, and mm can be a multiple of 5, if any.
            const u32 mvMod5 = ((u32) mv) - 5 * ((u32) div5(mv));
            if (mvMod5 == 0) {
                vrIsTrailingZeros = multipleOfPowerOf5(mv, q);
            } else if (acceptBounds) {
                // Same as min(e2 + (~mm & 1), pow5Factor(mm)) >= q
                // <=> e2 + (~mm & 1) >= q && pow5Factor(mm) >= q
                // <=> true && pow5Factor(mm) >= q, since e2 >= q.
                vmIsTrailingZeros = multipleOfPowerOf5(mv - 1 - mmShift, q);
            } else {
                // Same as min(e2 + 1, pow5Factor(mp)) >= q.
                vp -= multipleOfPowerOf5(mv + 2, q);
            }
        }
    } else {
        // This expression is slightly faster than max(0, log10Pow5(-e2) - 1).
        const u32 q = log10Pow5(-e2) - (-e2 > 1);
        e10 = (int32_t) q + e2;
        const int32_t i = -e2 - (int32_t) q;
        const int32_t k = pow5bits(i) - DOUBLE_POW5_BITCOUNT;
        const int32_t j = (int32_t) q - k;
        vr = mulShiftAll64(m2, DOUBLE_POW5_SPLIT[i], j, &vp, &vm, mmShift);
        if (q <= 1) {
            // {vr,vp,vm} is trailing zeros if {mv,mp,mm} has at least q trailing 0 bits.
            // mv = 4 * m2, so it always has at least two trailing 0 bits.
            vrIsTrailingZeros = true;
            if (acceptBounds) {
                // mm = mv - 1 - mmShift, so it has 1 trailing 0 bit iff mmShift == 1.
                vmIsTrailingZeros = mmShift == 1;
            } else {
                // mp = mv + 2, so it always has at least one trailing 0 bit.
                --vp;
            }
        } else if (q < 63) { // TODO(ulfjack): Use a tighter bound here.
            // We want to know if the full product has at least q trailing zeros.
            // We need to compute min(p2(mv), p5(mv) - e2) >= q
            // <=> p2(mv) >= q && p5(mv) - e2 >= q
            // <=> p2(mv) >= q (because -e2 >= q)
            vrIsTrailingZeros = multipleOfPowerOf2(mv, q);
        }
    }
    // Step 4: Find the shortest decimal representation in the interval of valid representations.
    int32_t removed = 0;
    uint8_t lastRemovedDigit = 0;
    u64 output;
    // On average, we remove ~2 digits.
    if (vmIsTrailingZeros || vrIsTrailingZeros) {
        // General case, which happens rarely (~0.7%).
        for (;;) {
            const u64 vpDiv10 = div10(vp);
            const u64 vmDiv10 = div10(vm);
            if (vpDiv10 <= vmDiv10) {
                break;
            }
            const u32 vmMod10 = ((u32) vm) - 10 * ((u32) vmDiv10);
            const u64 vrDiv10 = div10(vr);
            const u32 vrMod10 = ((u32) vr) - 10 * ((u32) vrDiv10);
            vmIsTrailingZeros &= vmMod10 == 0;
            vrIsTrailingZeros &= lastRemovedDigit == 0;
            lastRemovedDigit = (uint8_t) vrMod10;
            vr = vrDiv10;
            vp = vpDiv10;
            vm = vmDiv10;
            ++removed;
        }
        if (vmIsTrailingZeros) {
            for (;;) {
                const u64 vmDiv10 = div10(vm);
                const u32 vmMod10 = ((u32) vm) - 10 * ((u32) vmDiv10);
                if (vmMod10 != 0) {
                    break;
                }
                const u64 vpDiv10 = div10(vp);
                const u64 vrDiv10 = div10(vr);
                const u32 vrMod10 = ((u32) vr) - 10 * ((u32) vrDiv10);
                vrIsTrailingZeros &= lastRemovedDigit == 0;
                lastRemovedDigit = (uint8_t) vrMod10;
                vr = vrDiv10;
                vp = vpDiv10;
                vm = vmDiv10;
                ++removed;
            }
        }
        if (vrIsTrailingZeros && lastRemovedDigit == 5 && vr % 2 == 0) {
            // Round even if the exact number is .....50..0.
            lastRemovedDigit = 4;
        }
        // We need to take vr + 1 if vr is outside bounds or we need to round up.
        output = vr + ((vr == vm && (!acceptBounds || !vmIsTrailingZeros)) || lastRemovedDigit >= 5);
    } else {
        // Specialized for the common case (~99.3%). Percentages below are relative to this.
        bool roundUp = false;
        const u64 vpDiv100 = div100(vp);
        const u64 vmDiv100 = div100(vm);
        if (vpDiv100 > vmDiv100) { // Optimization: remove two digits at a time (~86.2%).
            const u64 vrDiv100 = div100(vr);
            const u32 vrMod100 = ((u32) vr) - 100 * ((u32) vrDiv100);
            roundUp = vrMod100 >= 50;
            vr = vrDiv100;
            vp = vpDiv100;
            vm = vmDiv100;
            removed += 2;
        }
        // Loop iterations below (approximately), without optimization above:
        // 0: 0.03%, 1: 13.8%, 2: 70.6%, 3: 14.0%, 4: 1.40%, 5: 0.14%, 6+: 0.02%
        // Loop iterations below (approximately), with optimization above:
        // 0: 70.6%, 1: 27.8%, 2: 1.40%, 3: 0.14%, 4+: 0.02%
        for (;;) {
            const u64 vpDiv10 = div10(vp);
            const u64 vmDiv10 = div10(vm);
            if (vpDiv10 <= vmDiv10) {
                break;
            }
            const u64 vrDiv10 = div10(vr);
            const u32 vrMod10 = ((u32) vr) - 10 * ((u32) vrDiv10);
            roundUp = vrMod10 >= 5;
            vr = vrDiv10;
            vp = vpDiv10;
            vm = vmDiv10;
            ++removed;
        }
        // We need to take vr + 1 if vr is outside bounds or we need to round up.
        output = vr + (vr == vm || roundUp);
    }
    const int32_t exp = e10 + removed;

    decfloat_t fd;
    fd.sign = sign;
    fd.exponent = output? exp : 0;
    fd.mantissa = output;
    return fd;
}

static decfloat_t f64_to_decimal(double d) {
    u64 bits = F64_BITS(d);
    u64 ieee_exp = F64_BEXP(bits);
    u64 ieee_mant = F64_MANT(bits);
    u64 sign = bits >> 63;
    return ieee_to_decimal(sign, ieee_mant, ieee_exp);
}
