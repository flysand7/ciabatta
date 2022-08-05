

static f64 strtod_generic(const char *restrict nptr, char **restrict endptr) {
    const char *restrict str = nptr;
    bool conv_performed = false;
    f64 value = 0.0;
    f64 coef = 1.0;
    // Skip space on the beginning
    while(isspace(*str)) {
        ++str;
    }
    // Check for inf and nan
    if(strpfx_i(str, "INF")) {
        str += sizeof "INF"-1;
        value = HUGE_VAL;
        conv_performed = true;
        goto finish;
    }
    if(strpfx_i(str, "INFINITY")) {
        str += sizeof "INFINITY"-1;
        value = HUGE_VAL;
        conv_performed = true;
        goto finish;
    }
    if(strpfx_i(str, "NAN")) {
        str += sizeof "NAN"-1;
        value = NAN;
        conv_performed = true;
        if(*str == '(') {
            while(*str != ')') {
                ++str;
            }
            ++str;
        }
        goto finish;
    }
    // Parse C float
    if(*str == '+') {
        ++str;
    }
    else if(*str == '-') {
        coef = -1.;
        ++str;
    }
    int base = 10;
    if(strpfx_i(str, "0X")) {
        str += sizeof "0X"-1;
        base = 16;
    }
    // Parse the whole part
    while(isbase(*str, base)) {
        long digit = todigit(*str);
        value = 10.0*value + (f64)digit;
        ++str;
    }
    if(*str != '.') {
        value = 0.0;
        goto finish;
    }
    ++str;
    // Parse the fractional part
    long exp = 1;
    while(isbase(*str, base)) {
        long digit = todigit(*str);
        f64 fract = (f64)digit;
        long cexp = exp;
        while(cexp-- != 0) {
            fract /= (f64)base;
        }
        value += fract;
        ++exp;
        ++str;
    }
    // Parse the exponent
    if((base == 10 && strpfx_i(str, "E"))
       || (base == 16 && strpfx_i(str, "P")))
    {
        ++str;
        long exp = 0;
        long exp_coef = 1;
        if(*str == '+') {
            ++str;
        }
        else if(*str == '-') {
            exp_coef = -1;
            ++str;
        }
        while(isdigit(*str)) {
            exp = 10*exp + (long)(*str-'0');
            ++str;
        }
        if(exp_coef == 1) {
            while(exp--!=0) value = value * base;
        }
        else if(exp_coef == -1) {
            while(exp--!=0) value = value / base;
        }
    }
    if(!isfinite(value)) {
        errno = ERANGE;
        value = coef*HUGE_VAL;
    }
    conv_performed = true;
    finish:
    if(endptr != NULL) {
        if(conv_performed) {
            *endptr = (char *)str;
        }
        else {
            *endptr = (char *)nptr;
        }
    }
    return coef*value;
}

f64 strtod(const char *restrict nptr, char **restrict endptr) {
    return strtod_generic(nptr, endptr);
}

f32 strtof(const char *restrict nptr, char **restrict endptr) {
    return (f32)strtod_generic(nptr, endptr);
}

fl64 strtold(const char *restrict nptr, char **restrict endptr) {
    return (fl64)strtod_generic(nptr, endptr);
}

f64 atof(const char *nptr) {
    return strtod(nptr, (char **)NULL);
}
