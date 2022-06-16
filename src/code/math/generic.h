
ftype suffix(exp)(ftype x) {
    if(isnan(x)) return NAN;
    if(x > 0 && isinf(x)) return +INFINITY;
    if(x < 0 && isinf(x)) return +0.0;
    if(x == 0) return 1.0;
    if(x > 709.8) {
        #if math_errhandling & MATH_ERREXCEPT
            feraiseexcept(FE_OVERFLOW);
        #endif
        #if math_errhandling & MATH_ERRNO
            errno = ERANGE;
        #endif
        return +INFINITY;
    }
    if(x < -708.4) {
        #if math_errhandling & MATH_ERREXCEPT
            feraiseexcept(FE_OVERFLOW);
        #endif
        #if math_errhandling & MATH_ERRNO
            errno = ERANGE;
        #endif
        return 0;
    }
    ftype e = 1.0;
    ftype xp = 1.0;
    ftype f = 1;
    for(uint64_t i = 1; i != 10; ++i) {
        f *= i;
        xp *= x;
        e += xp / f;
    }
    return e;
}

ftype suffix(exp2)(ftype x) {
    return suffix(exp)(x * ln2);
}

ftype suffix(expm1)(ftype x) {
    return suffix(exp)(x) - 1.0;
}
