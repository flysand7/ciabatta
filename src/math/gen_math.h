

static ftype suffix(cordic)(
    ftype x0,  // x initial value
    ftype y0,  // y initial value
    ftype z0,  // initial 'angle'
    int m,     // system: 1 for trig, -1 for hyperbolic
    int v,     // mode: 1 for vectoring, 0 for rotation
    ftype *c,  // output x
    ftype *s   // output y
) {
    int tab_count = 0;
    ftype *tab = NULL;
    if(m == 1) {
        tab = suffix(ttable);
        tab_count = countof(suffix(ttable));
    }
    else if(m == -1) {
        tab = suffix(htable);
        tab_count = countof(suffix(htable));
    }
    ftype x = x0;
    ftype y = y0;
    ftype z = z0;
    for(int i = 0; i != tab_count; ++i) {
        ftype sign;
        if(v) sign = (y < 0)?  1 : -1;
        else  sign = (z < 0)? -1 :  1;
        ftype x1 = suffix(fma)(suffix(ptable)[i], -m*sign*y, x);
        ftype y1 = suffix(fma)(suffix(ptable)[i], x*sign, y);
        x = x1;
        y = y1;
        z = suffix(fma)(tab[i], -sign, z);
    }
    if(c!=NULL) *c = x;
    if(s!=NULL) *s = y;
    return z;
}

ftype suffix(sin)(ftype x) {
    if(isinf(x)) return NAN;
    if(isnan(x)) return NAN;
    int k;
    ftype alpha = suffix(remquo)(x, HALF_PI, &k);
    if(x == 0) return x;
    k = (((k % 4) +4) %4);
    ftype sinx;
    ftype cosx;
    suffix(cordic)(suffix(tK), 0, alpha, 1, 0, &cosx, &sinx);
    switch(k) {
        case 0: return sinx;
        case 1: return cosx;
        case 2: return -sinx;
        case 3: return -cosx;
    }
    return 0;
}

ftype suffix(cos)(ftype x) {
    if(isinf(x)) return NAN;
    if(isnan(x)) return NAN;
    int k;
    ftype alpha = suffix(remquo)(x, HALF_PI, &k);
    k = (((k % 4) +4) %4);
    ftype sinx;
    ftype cosx;
    suffix(cordic)(suffix(tK), 0, alpha, 1, 0, &cosx, &sinx);
    switch(k) {
        case 0: return cosx;
        case 1: return -sinx;
        case 2: return -cosx;
        case 3: return sinx;
    }
    return 0;
}

ftype suffix(tan)(ftype x) {
    if(isinf(x)) return NAN;
    if(isnan(x)) return NAN;
    int k;
    ftype alpha = suffix(remquo)(x, HALF_PI, &k);
    if(x == 0) return x;
    k = (((k % 2) +2) %2);
    ftype sinx;
    ftype cosx;
    suffix(cordic)(suffix(tK), 0, alpha, 1, 0, &cosx, &sinx);
    switch(k) {
        case 0: return sinx/cosx;
        case 1: return -cosx/sinx;
    }
    return 0;
}

ftype suffix(cot)(ftype x) {
    if(x == 0) return x;
    if(isinf(x)) return NAN;
    if(isnan(x)) return NAN;
    int k;
    ftype alpha = suffix(remquo)(x, HALF_PI, &k);
    k = (((k % 2) +2) %2);
    ftype sinx;
    ftype cosx;
    suffix(cordic)(suffix(tK), 0, alpha, 1, 0, &cosx, &sinx);
    switch(k) {
        case 0: return cosx/sinx;
        case 1: return -sinx/cosx;
    }
    return 0;
}

ftype suffix(exp)(ftype x) {
    if(x == 0) return x;
    if(isinf(x)) return NAN;
    if(isnan(x)) return NAN;
    ftype t = x*LOG2E;
    int64_t k = (int64_t)t;
    x = (t - (ftype)k) / LOG2E;
    ftype xx = x*x;
    ftype xxx = xx*x;
    ftype xxxx = xx*xx;
    ftype xxxxx = xxx*xx;
    ftype expx = 1+x + xx/2.0 + xxx/6.0+ xxxx/24.0 + xxxxx/720.0;
    if(k>0) expx *= 2.0;
    if(k>0) while(k-- > 0) expx *= 2.0;
    if(k<0) while(k++ < 0) expx /= 2.0;
    return expx;
}

ftype suffix(atan)(ftype x) {
    if(x == 0)   return x;
    if(isinf(x)) return INFINITY;
    if(isnan(x)) return NAN;
    ftype atan;
    if(x > 1) {
        atan = HALF_PI - suffix(cordic)(x, 1, 0, 1, 1, NULL, NULL);
    }
    else if(x < -1) {
        atan = -HALF_PI + suffix(cordic)(-x, 1, 0, 1, 1, NULL, NULL);
    }
    else {
        atan = suffix(cordic)(1, x, 0, 1, 1, NULL, NULL);
    }
    return atan;
}

ftype suffix(acos)(ftype x) {
    if(x ==  0) return HALF_PI;
    if(x == -1) return PI;
    if(x ==  1) return 0;
    if(isinf(x)) return INFINITY;
    if(isnan(x)) return NAN;
    if(fabs(x) > 1) return NAN;
    ftype atan;
    x = sqrt(fma(-x,x,1))/x;
    if(x > 1) {
        atan = HALF_PI - suffix(cordic)(x, 1, 0, 1, 1, NULL, NULL);
    }
    else if(x < -1) {
        atan = -HALF_PI + suffix(cordic)(-x, 1, 0, 1, 1, NULL, NULL);
    }
    else {
        atan = suffix(cordic)(1, x, 0, 1, 1, NULL, NULL);
    }
    if(x < 0) atan += PI;
    return atan;
}

ftype suffix(asin)(ftype x) {
    if(x ==  0) return 0;
    if(x == -1) return -HALF_PI;
    if(x ==  1) return HALF_PI;
    if(isinf(x)) return INFINITY;
    if(isnan(x)) return NAN;
    if(fabs(x) > 1) return NAN;
    ftype atan;
    x /= sqrt(fma(-x,x,1));
    if(x > 1) {
        atan = HALF_PI - suffix(cordic)(x, 1, 0, 1, 1, NULL, NULL);
    }
    else if(x < -1) {
        atan = -HALF_PI + suffix(cordic)(-x, 1, 0, 1, 1, NULL, NULL);
    }
    else {
        atan = suffix(cordic)(1, x, 0, 1, 1, NULL, NULL);
    }
    return atan;
}

// ftype suffix(exp)(ftype x) {
//     if(isnan(x)) return NAN;
//     if(x > 0 && isinf(x)) return +INFINITY;
//     if(x < 0 && isinf(x)) return +0.0;
//     if(x == 0) return 1.0;
//     if(x > 709.8) {
//         #if math_errhandling & MATH_ERREXCEPT
//             feraiseexcept(FE_OVERFLOW);
//         #endif
//         #if math_errhandling & MATH_ERRNO
//             errno = ERANGE;
//         #endif
//         return +INFINITY;
//     }
//     if(x < -708.4) {
//         #if math_errhandling & MATH_ERREXCEPT
//             feraiseexcept(FE_OVERFLOW);
//         #endif
//         #if math_errhandling & MATH_ERRNO
//             errno = ERANGE;
//         #endif
//         return 0;
//     }
//     ftype e = 1.0;
//     ftype xp = 1.0;
//     ftype f = 1;
//     for(uint64_t i = 1; i != 10; ++i) {
//         f *= i;
//         xp *= x;
//         e += xp / f;
//     }
//     return e;
// }

ftype suffix(exp2)(ftype x) {
    return suffix(exp)(x * LN2);
}

ftype suffix(expm1)(ftype x) {
    return suffix(exp)(x) - 1.0;
}
