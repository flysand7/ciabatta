
#include <math.h>
#include <stdint.h>

double remquo(double x, double y, int *quo) {
    union {double f; uint64_t i;} ux = {x}, uy = {y};
    int ex = ux.i>>52 & 0x7ff;
    int ey = uy.i>>52 & 0x7ff;
    int sx = ux.i>>63;
    int sy = uy.i>>63;
    uint32_t q;
    uint64_t i;
    uint64_t uxi = ux.i;

    *quo = 0;
    if (uy.i<<1 == 0 || isnan(y) || ex == 0x7ff)
        return (x*y)/(x*y);
    if (ux.i<<1 == 0)
        return x;

    /* normalize x and y */
    if (!ex) {
        for (i = uxi<<12; i>>63 == 0; ex--, i <<= 1);
        uxi <<= -ex + 1;
    } else {
        uxi &= -1ULL >> 12;
        uxi |= 1ULL << 52;
    }
    if (!ey) {
        for (i = uy.i<<12; i>>63 == 0; ey--, i <<= 1);
        uy.i <<= -ey + 1;
    } else {
        uy.i &= -1ULL >> 12;
        uy.i |= 1ULL << 52;
    }

    q = 0;
    if (ex < ey) {
        if (ex+1 == ey)
            goto end;
        return x;
    }

    /* x mod y */
    for (; ex > ey; ex--) {
        i = uxi - uy.i;
        if (i >> 63 == 0) {
            uxi = i;
            q++;
        }
        uxi <<= 1;
        q <<= 1;
    }
    i = uxi - uy.i;
    if (i >> 63 == 0) {
        uxi = i;
        q++;
    }
    if (uxi == 0)
        ex = -60;
    else
        for (; uxi>>52 == 0; uxi <<= 1, ex--);
end:
    /* scale result and decide between |x| and |x|-|y| */
    if (ex > 0) {
        uxi -= 1ULL << 52;
        uxi |= (uint64_t)ex << 52;
    } else {
        uxi >>= -ex + 1;
    }
    ux.i = uxi;
    x = ux.f;
    if (sy)
        y = -y;
    if (ex == ey || (ex+1 == ey && (2*x > y || (2*x == y && q%2)))) {
        x -= y;
        q++;
    }
    q &= 0x7fffffff;
    *quo = sx^sy ? -(int)q : (int)q;
    return sx ? -x : x;
}

float remquof(float x, float y, int *quo) {
    union {float f; uint32_t i;} ux = {x}, uy = {y};
    int ex = ux.i>>23 & 0xff;
    int ey = uy.i>>23 & 0xff;
    int sx = ux.i>>31;
    int sy = uy.i>>31;
    uint32_t q;
    uint32_t i;
    uint32_t uxi = ux.i;

    *quo = 0;
    if (uy.i<<1 == 0 || isnan(y) || ex == 0xff)
        return (x*y)/(x*y);
    if (ux.i<<1 == 0)
        return x;

    /* normalize x and y */
    if (!ex) {
        for (i = uxi<<9; i>>31 == 0; ex--, i <<= 1);
        uxi <<= -ex + 1;
    } else {
        uxi &= -1U >> 9;
        uxi |= 1U << 23;
    }
    if (!ey) {
        for (i = uy.i<<9; i>>31 == 0; ey--, i <<= 1);
        uy.i <<= -ey + 1;
    } else {
        uy.i &= -1U >> 9;
        uy.i |= 1U << 23;
    }

    q = 0;
    if (ex < ey) {
        if (ex+1 == ey)
            goto end;
        return x;
    }

    /* x mod y */
    for (; ex > ey; ex--) {
        i = uxi - uy.i;
        if (i >> 31 == 0) {
            uxi = i;
            q++;
        }
        uxi <<= 1;
        q <<= 1;
    }
    i = uxi - uy.i;
    if (i >> 31 == 0) {
        uxi = i;
        q++;
    }
    if (uxi == 0)
        ex = -30;
    else
        for (; uxi>>23 == 0; uxi <<= 1, ex--);
end:
    /* scale result and decide between |x| and |x|-|y| */
    if (ex > 0) {
        uxi -= 1U << 23;
        uxi |= (uint32_t)ex << 23;
    } else {
        uxi >>= -ex + 1;
    }
    ux.i = uxi;
    x = ux.f;
    if (sy)
        y = -y;
    if (ex == ey || (ex+1 == ey && (2*x > y || (2*x == y && q%2)))) {
        x -= y;
        q++;
    }
    q &= 0x7fffffff;
    *quo = sx^sy ? -(int)q : (int)q;
    return sx ? -x : x;
}

long double remquol(long double x, long double y, int *quo) {
    return remquo(x, y, quo);
}

double remainder(double x, double y) {
    int q;
    return remquo(x, y, &q);
}

float remainderf(float x, float y) {
    int q;
    return remquof(x, y, &q);
}

long double remainderl(long double x, long double y) {
    return remainder(x, y);
}

double modf(double x, double *iptr) {
    union {double f; uint64_t i;} u = {x};
    uint64_t mask;
    int e = (int)(u.i>>52 & 0x7ff) - 0x3ff;

    /* no fractional part */
    if (e >= 52) {
        *iptr = x;
        if (e == 0x400 && u.i<<12 != 0) /* nan */
            return x;
        u.i &= 1ULL<<63;
        return u.f;
    }

    /* no integral part*/
    if (e < 0) {
        u.i &= 1ULL<<63;
        *iptr = u.f;
        return x;
    }

    mask = -1ULL>>12>>e;
    if ((u.i & mask) == 0) {
        *iptr = x;
        u.i &= 1ULL<<63;
        return u.f;
    }
    u.i &= ~mask;
    *iptr = u.f;
    return x - u.f;
}

float modff(float x, float *iptr) {
    union {float f; uint32_t i;} u = {x};
    uint32_t mask;
    int e = (int)(u.i>>23 & 0xff) - 0x7f;

    /* no fractional part */
    if (e >= 23) {
        *iptr = x;
        if (e == 0x80 && u.i<<9 != 0) { /* nan */
            return x;
        }
        u.i &= 0x80000000;
        return u.f;
    }
    /* no integral part */
    if (e < 0) {
        u.i &= 0x80000000;
        *iptr = u.f;
        return x;
    }

    mask = 0x007fffff>>e;
    if ((u.i & mask) == 0) {
        *iptr = x;
        u.i &= 0x80000000;
        return u.f;
    }
    u.i &= ~mask;
    *iptr = u.f;
    return x - u.f;
}

long double modfl(long double x, long double *iptr) {
    double d;
    long double r = modf(x, &d);
    *iptr = d;
    return r;
}
