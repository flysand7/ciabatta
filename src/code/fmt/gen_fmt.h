
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

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
static inline int pfx(_out_rbuf)(
    int w,
    void *ctx,
    pfx(cbfn) cb,
    size_t len,
    ctype *buf,
    int width,
    int flags
);
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
            if(!cb(ctx, *fmt))
                return -1;
            ++w;
            ++fmt;
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
        case 'b':
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
            break;
        case 'f': ;
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
    // Print left-pad due to width
    if(!(flags & FLAG_LEFT)) {
        ctype pad_ch = ' ';
        if(flags & FLAG_ZERO) pad_ch = '0';
        while(pad_len-- > 0) {
            if(!cb(ctx, pad_ch)) return -1;
            ++w;
        }
    }
    // Print prefix
    if(flags & FLAG_HASH) {
        if((++w, !cb(ctx, '0'))) return -1;
        if(base == 16)     { if((++w, !cb(ctx, 'x'))) return -1; }
        else if(base == 2) { if((++w, !cb(ctx, 'b'))) return -1; }
    }
    else if(neg)                { if((++w, !cb(ctx, '-'))) return -1; }
    else if(flags & FLAG_PLUS)  { if((++w, !cb(ctx, '+'))) return -1; }
    else if(flags & FLAG_SPACE) { if((++w, !cb(ctx, ' '))) return -1; }
    // Print zero-pad due to precision
    for(int i = ndigits; i < prec; ++i) {
        if(!cb(ctx, '0')) return -1;
        ++w;
    }
    // Output buffer in reverse
    if(ndigits) while(ndigits--) {
        if(!cb(ctx, digits[ndigits])) return -1;
        ++w;
    }
    // Print right-pad
    if(flags & FLAG_LEFT) {
        while(pad_len-- > 0) {
            if(!cb(ctx, ' ')) return -1;
            ++w;
        }
    }
    return w;
}
