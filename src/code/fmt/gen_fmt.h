
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include <ryu/ryu.h>

typedef int (pfx(cbfn))(void *ctx, ctype ch);

#define FLAG_ZERO   0x0001 // "0"
#define FLAG_LEFT   0x0002 // "-"
#define FLAG_PLUS   0x0004 // "+"
#define FLAG_SPACE  0x0008 // " "
#define FLAG_HASH   0x0010 // "#"
#define FLAG_PREC   0x0020 // ".precision"
#define FLAG_LONG   0x0040 // "l"
#define FLAG_LLONG  0x0080 // "ll"
#define FLAG_SHORT  0x0100 // "h"
#define FLAG_CHAR   0x0200 // "hh"
#define FLAG_UPPER  0x0400 // for hex digits 0xa-0xf

// Helper function declaration
static inline int pfx(_isdigit)(ctype ch);
static inline int pfx(_atoi)(ctype const **sp, int *err);
static inline int pfx(_ntoa)(
    int w,
    void *ctx,
    pfx(cbfn) cb,
    int neg,
    unsigned long long value,
    int base,
    int prec,
    int width,
    int flags
);
static inline int pfx(_dtoh)(
    int w,
    void *ctx,
    pfx(cbfn) cb,
    double value,
    int prec,
    int width,
    int flags
);
static inline int pfx(_dtoa)(
    int w,
    void *ctx,
    pfx(cbfn) cb,
    double value,
    int prec,
    int width,
    int flags
);
static inline int pfx(_etoa)(
    int w,
    void *ctx,
    pfx(cbfn) cb,
    double value,
    int prec,
    int width,
    int flags
);
static inline int pfx(_infnantoa)(
    int w,
    void *ctx,
    pfx(cbfn) cb,
    double value,
    int prec,
    int width,
    int flags
);

#define out(ch) do { if((w++, !cb(ctx, (ch)))) return -1; } while(0)

// Generic print
static int pfx(vprintfcb)(
    void *ctx,
    pfx(cbfn) *cb,
    const ctype *fmt,
    va_list va
) {
    int w = 0;
    while(*fmt) {
        // Write a character if it's not a '%' sign
        while(*fmt && *fmt != '%') {
            out(*fmt++);
            continue;
        }
        if(*fmt == '%') {
            ++fmt;
        }
        // Parse flags
        int flags = 0;
        int isflag = 0;
        do {
            isflag = 1;
            switch(*fmt) {
                case '0': flags |= FLAG_ZERO;  break;
                case '-': flags |= FLAG_LEFT;  break;
                case '+': flags |= FLAG_PLUS;  break;
                case ' ': flags |= FLAG_SPACE; break;
                case '#': flags |= FLAG_HASH;  break;
                default:  isflag = 0;
            }
            if(isflag) ++fmt;
        } while(isflag);
        if(flags & FLAG_LEFT) flags &= ~FLAG_ZERO;
        // Parse width field
        int width = 0;
        if(pfx(_isdigit(*fmt))) {
            int err = 0;
            width = pfx(_atoi(&fmt, &err));
            if(err) return -1;
        }
        else if(*fmt == '*') {
            ++fmt;
            width = va_arg(va, int);
            if(width < 0) {
                width = -width;
                flags |= FLAG_LEFT;
            }
        }
        // If present, parse the precision field
        int prec = 0;
        if(*fmt == '.') {
            flags |= FLAG_PREC;
            ++fmt;
            if(pfx(_isdigit(*fmt))) {
                int err = 0;
                prec = pfx(_atoi(&fmt, &err));
                if(err) return -1;
            }
            else if(*fmt == '*') {
                ++fmt;
                prec = va_arg(va, int);
                prec = (prec > 0? prec : 0);
            }
        }
        // Parse length field
        switch(*fmt) {
        case 'l':
            ++fmt;
            flags |= FLAG_LONG;
            if(*fmt == 'l') {
                ++fmt;
                flags |= FLAG_LLONG;
            }
            break;
        case 'h':
            ++fmt;
            flags |= FLAG_SHORT;
            if(*fmt == 'h') {
                ++fmt;
                flags |= FLAG_CHAR;
            }
            break;
        case 'j':
            ++fmt;
            if(sizeof(intmax_t) == sizeof(long))
                 flags |= FLAG_LONG;
            else flags |= FLAG_LLONG;
            break;
        case 'z':
            ++fmt;
            if(sizeof(size_t) == sizeof(long))
                 flags |= FLAG_LONG;
            else flags |= FLAG_LLONG;
            break;
        case 't':
            ++fmt;
            if(sizeof(ptrdiff_t) == sizeof(long))
                 flags |= FLAG_LONG;
            else flags |= FLAG_LLONG;
            break;
        case 'L':
            ++fmt;
            break;
        default:
            break;
        }
        // Print according to specifier
        switch(*fmt) {
        case 'd':
        case 'i':
        case 'u':
        case 'x':
        case 'X':
        case 'o':
        case 'b': {
            // Figure out the signedness and base
            int base = 10;
            if(*fmt == 'x' || *fmt == 'X') base = 16;
            if(*fmt == 'o') base = 8;
            if(*fmt == 'b') base = 2;
            int signd = (*fmt == 'i' || *fmt == 'd');
            // Figure out flags
            if(*fmt == 'X')       flags |= FLAG_UPPER;
            if(base == 10)        flags &= ~FLAG_HASH;
            if(!signd)            flags &= ~(FLAG_PLUS|FLAG_SPACE);
            if(flags & FLAG_PREC) flags &= ~FLAG_ZERO;
            ++fmt;
            // Read-in the integer argument from var args
            int neg = 0;
            unsigned long long value = 0;
            if(signd) {
                long long num;
                if(flags & FLAG_LLONG)
                    num = (long long)va_arg(va, long long);
                else if(flags & FLAG_LONG)
                    num = (long long)va_arg(va, long);
                else
                    num = (long long)va_arg(va, int);
                // Avoiding UB on unary negation like a bitch
                if(num == LLONG_MIN) {
                    neg = 1;
                    value = (unsigned long long)LLONG_MAX + 1;
                }
                else {
                    neg = num >= 0?   0 :    1;
                    num = num >= 0? num : -num;
                    value = (unsigned long long)num;
                }
            }
            else {
                if(flags & FLAG_LLONG)
                    value = (unsigned long long)va_arg(va, unsigned long long);
                else if(flags & FLAG_LONG)
                    value = (unsigned long long)va_arg(va, unsigned long);
                else
                    value = (unsigned long long)va_arg(va, unsigned int);
            }
            // Output integer
            w = pfx(_ntoa)(w, ctx, cb, neg, value, base, prec, width, flags);
            if(w < 0) return -1;
        } break;
        case 'a':
        case 'A': {
            if(*fmt == 'A') flags |= FLAG_UPPER;
            ++fmt;
            double value = va_arg(va, double);
            w = pfx(_dtoh)(w, ctx, cb, value, prec, width, flags);
            if(w < 0) return -1;
        } break;
        case 'e':
        case 'E':
        case 'f':
        case 'F':
        case 'g':
        case 'G':
            if(*fmt == 'E' || *fmt == 'F' || *fmt == 'G') flags |= FLAG_UPPER;
            char conv = tolower(*fmt);
            ++fmt;
            double value = va_arg(va, double);
            if(conv == 'f') {
                w = pfx(_dtoa)(w, ctx, cb, value, prec, width, flags);
            }
            else if(conv == 'e') {
                w = pfx(_etoa)(w, ctx, cb, value, prec, width, flags);
            }
            else {
                int P = prec;
                if(!(flags & FLAG_PREC)) P = 6;
                if(prec == 0) P = 1;
                union {
                    uint64_t bits;
                    double value;
                } _ = {.value = value};
                uint64_t bits = _.bits;
                int E = (int)((bits >> 52) & 0x7ff) - 1023;
                int class = fpclassify(value);
                if(class == FP_SUBNORMAL || class == FP_ZERO) {
                    E = 0;
                }
                if(P > E && E >= -4) {
                    w = pfx(_dtoa)(w, ctx, cb, value, P-(E+1), width, flags);
                }
                else {
                    w = pfx(_etoa)(w, ctx, cb, value, P-1, width, flags);
                }
            }
            if(w < 0) return -1;
            break;
        }
    }
    return w;
}

static inline int pfx(_isdigit)(ctype ch) {
    return '0' <= ch && ch <= '9';
}

static inline int pfx(_atoi)(ctype const **sp, int *err) {
    int i = 0;
    ctype const *s = *sp;
    while(pfx(_isdigit(*s))) {
        int d = *s-'0';
        if(i > (INT_MAX - d)/10) {
            *err = 1;
            return 0;
        }
        i = 10*i + d;
        ++s;
    }
    *sp = s;
    return i;
}

static inline int pfx(_ntoa)(
    int w,
    void *ctx,
    pfx(cbfn) cb,
    int neg,
    unsigned long long value,
    int base,
    int prec,
    int width,
    int flags
) {
    // We'll Write digits into buf in reverse and then call _out_rbuf
    int ndigits = 0;
    static ctype digits[64];
    // Remove hash flag for 0 values
    if(value == 0) flags &= ~FLAG_HASH;
    // Write digits to buffer in reverse (starting from least significant)
    while(value) {
        int d = (int)(value%base);
        value /= base;
        if(d < 10)
             d += '0';
        else if(flags & FLAG_UPPER)
             d += 'A' - 10;
        else d += 'a' - 10;
        digits[ndigits++] = d;
    }
    // Figure out the length of the prefix (part of number before digit stirng)
    int pref_len = 0;
    if(flags & FLAG_HASH)       pref_len = base == 8? 1 : 2;
    else if(neg)                pref_len = 1;
    else if(flags & FLAG_PLUS)  pref_len = 1;
    else if(flags & FLAG_SPACE) pref_len = 1;
    if(ndigits > prec) prec = ndigits;
    int num_len = pref_len + prec;
    int pad_len = width - num_len;
    // Print left-pad due to width (TODO: zero pad should come after prefix?)
    if(!(flags & FLAG_LEFT)) {
        ctype pad_ch = ' ';
        if(flags & FLAG_ZERO) pad_ch = '0';
        while(pad_len-- > 0) {
            out(pad_ch);
        }
    }
    // Print prefix
    if(flags & FLAG_HASH) {
        out('0');
        if(base == 16)     { out('x'); }
        else if(base == 2) { out('b'); }
    }
    else if(neg)                { out('-'); }
    else if(flags & FLAG_PLUS)  { out('+'); }
    else if(flags & FLAG_SPACE) { out(' '); }
    // Print zero-pad due to precision
    for(int i = ndigits; i < prec; ++i) {
        out('0');
    }
    // Output buffer in reverse
    if(ndigits) while(ndigits--) {
        out(digits[ndigits]);
    }
    // Print right-pad
    if(flags & FLAG_LEFT) {
        while(pad_len-- > 0) {
            out(' ');
        }
    }
    return w;
}

static inline int pfx(_infnantoa)(
    int w,
    void *ctx,
    pfx(cbfn) cb,
    double value,
    int prec,
    int width,
    int flags
) {
    char const *name = NULL;
    int nchars = 0;
    if(isinf(value)) name = "inf", nchars = 3;
    if(isnan(value)) name = "nan", nchars = 3;
    // Figure out prefix
    int pref_len = 0;
    union {
        uint64_t bits;
        double value;
    } _ = {value};
    uint64_t bits = _.bits;
    uint64_t neg = bits >> 63;
    if(neg) pref_len = 1;
    else if(flags & FLAG_PLUS) pref_len = 1;
    else if(flags & FLAG_SPACE) pref_len = 1;
    // Figure out pad
    int str_len = pref_len + nchars;
    int pad_len = width - str_len;
    // Print left-pad
    if(!(flags & FLAG_LEFT)) {
        while(pad_len-- > 0) {
            out(' ');
        }
    }
    // Print the prefix
    if(neg)                     out('-');
    else if(flags & FLAG_PLUS)  out('+');
    else if(flags & FLAG_SPACE) out(' ');
    // Print the string
    while(*name) {
        out(*name++);
    }
    // Print right-pad
    if(flags & FLAG_LEFT) {
        while(pad_len-- > 0) {
            out(' ');
        }
    }
    return w;
}

static inline int pfx(_dtoh)(
    int w,
    void *ctx,
    pfx(cbfn) cb,
    double value,
    int prec,
    int width,
    int flags
) {
    int class = fpclassify(value);
    if(class == FP_INFINITE || class == FP_NAN) {
        return pfx(_infnantoa)(w, ctx, cb, value, prec, width, flags);
    }
    // Disassemble the float into parts
    union {
        uint64_t bits;
        double value;
    } _ = {.value = value};
    uint64_t bits = _.bits;
    uint64_t neg = bits >> 63;
    int64_t exp = (int64_t)((bits >> 52) & 0x7ff) - 1023;
    int64_t mant = bits & ((UINT64_C(1)<<51)-1);
    if(class == FP_SUBNORMAL || class == FP_ZERO) {
        exp = 0;
    }
    // Figure out how many hex digits does mantissa take up (mant_digits_n)
    // and the number of digits after decimal point (prec)
    static ctype mant_buf[64] = {0};
    int mant_digits_n;
    int nonsig_digits_n = 0;
    if(mant != 0) {
        int64_t m = mant;
        while((m & 0xf) == 0) {
            ++nonsig_digits_n;
            m >>= 4;
        }
    }
    else {
        nonsig_digits_n = 52/4;
    }
    mant_digits_n = 52/4 - nonsig_digits_n;
    if((flags & FLAG_PREC) == 0) {
        prec = mant_digits_n;
    }
    // Figure out how many symbols decimal point takes up (0 or 1)
    int decimal_point_n = 1;
    if(prec == 0) {
        decimal_point_n = 0;
        if(flags & FLAG_HASH) {
            decimal_point_n = 1;
        }
    }
    // Figure out how many digits exponent take up and it's sign
    // also save exponent digits to a buffer starting from LSD
    static ctype exp_buf[64] = {0};
    int exp_digits_n = 0;
    ctype exp_sign;
    if(exp < 0) {
        exp_sign = '-';
        exp = -exp;
    }
    else {
        exp_sign = '+';
    }
    {
        int64_t e = exp;
        do {
            exp_buf[exp_digits_n++] = e % 10 + '0';
            e /= 10;
        } while(e != 0);
    }
    // Figure out sign symbol and number of chars it takes up (0 or 1)
    int sign_n = 0;
    ctype sign_ch;
    if(value < 0) {
        sign_n = 1;
        sign_ch = '-';
    }
    else if(flags & FLAG_PLUS) {
        sign_n = 1;
        sign_ch = '+';
    }
    else if(flags & FLAG_SPACE) {
        sign_n = 1;
        sign_ch = ' ';
    }
    // Figure out the width of the number
    int significand_width = 1 + decimal_point_n + prec;
    int exp_part_width = 2 /*p+-*/ + exp_digits_n;
    int n_width = sign_n + 2 /*0x*/ + significand_width + exp_part_width;
    // Figure out width-padding required
    int pad = 0;
    if(width > n_width) pad = width - n_width;
    // Print width left-pad if it's made out of space
    if(!(flags & FLAG_LEFT) && !(flags & FLAG_ZERO)) while(pad-- > 0) {
        out(' ');
    }
    // Print sign if there
    if(sign_n)
        out(sign_ch);
    // Print 0x, 0X
    out('0');
    out((flags & FLAG_UPPER)? 'X' : 'x');
    // Print width left-pad if it's made out of zero
    if(!(flags & FLAG_LEFT) && (flags & FLAG_ZERO)) while(pad-- > 0) {
        out('0');
    }
    // Print whole part
    out((class == FP_SUBNORMAL || class == FP_ZERO)? '0' : '1');
    // Print decimal point
    if(decimal_point_n) out('.');
    // Print precision padding
    for(int i = mant_digits_n; i < prec; ++i) {
        out('0');
    }
    if(prec < mant_digits_n) mant_digits_n = prec;
    for(int i = 0; i != mant_digits_n; ++i) {
        int bit = 52 - 4 - i;
        int digit = (int)((mant >> bit) & 0xf);
        ctype ch;
        if(digit < 10)
             ch = digit+'0';
        else if(flags & FLAG_UPPER)
             ch = digit+'A'-10;
        else ch = digit+'a'-10;
        out(ch);
    }
    // Print the exponent part
    out((flags & FLAG_UPPER)? 'P' : 'p');
    out(exp_sign);
    if(exp_digits_n) do
        out(exp_buf[exp_digits_n]);
    while(exp_digits_n--);
    // Print right-pad
    if(flags & FLAG_LEFT) while(pad-- > 0) {
        out(' ');
    }
    return w;
}

static inline int pfx(_dtoa)(
    int w,
    void *ctx,
    pfx(cbfn) cb,
    double value,
    int prec,
    int width,
    int flags
) {
    int class = fpclassify(value);
    if(class == FP_INFINITE || class == FP_NAN) {
        return pfx(_infnantoa)(w, ctx, cb, value, prec, width, flags);
    }
    // This guy does memory allocation which makes it pretty cringe
    if(!(flags & FLAG_PREC)) prec = 6;
    char *buf = d2fixed(value, prec);
    int len = (int)strlen(buf);
    int pad = width - len;
    // Left pad
    if(!(flags & FLAG_LEFT) && !(flags & FLAG_ZERO)) while(pad-- > 0) {
        out(' ');
    }
    {
        char *str = buf;
        while(*str) {
            out(*str);
            ++str;
        }
    }
    free(buf);
    // Right pad
    if(flags & FLAG_LEFT) while(pad-- > 0) {
        out(' ');
    }
    return w;
}

static inline int pfx(_etoa)(
    int w,
    void *ctx,
    pfx(cbfn) cb,
    double value,
    int prec,
    int width,
    int flags
) {
    int class = fpclassify(value);
    if(class == FP_INFINITE || class == FP_NAN) {
        return pfx(_infnantoa)(w, ctx, cb, value, prec, width, flags);
    }
    // This guy does memory allocation which makes it pretty cringe
    if(!(flags & FLAG_PREC)) prec = 6;
    char *buf = d2exp(value, prec);
    int len = (int)strlen(buf);
    int pad = width - len;
    // Left pad
    if(!(flags & FLAG_LEFT) && !(flags & FLAG_ZERO)) while(pad-- > 0) {
        out(' ');
    }
    {
        char *str = buf;
        while(*str) {
            out(*str);
            ++str;
        }
    }
    free(buf);
    // Right pad
    if(flags & FLAG_LEFT) while(pad-- > 0) {
        out(' ');
    }
    return w;
}

#undef out
