
static intull strtoi_generic(const char *restrict nptr,
                             char **restrict endptr,
                             int inbase,
                             intl *coefptr,
                             intull int_max) {
    const char *restrict str = nptr;
    intull value = 0;
    int digits_read = 0;
    bool is_signed = (coefptr != NULL);
    intl coef = 1;
    // Find max{abs(int)}. Signed integers have negative,
    // whose absolute value is 1 bigger than int_max.
    intull int_abs_max = int_max;
    if(is_signed) {
        ++int_abs_max;
    }
    if(!IN_RANGE(0, inbase, 36)) {
        goto finish;
    }
    intull base = (intull)inbase;
    // Skip space on the beginning
    while(isspace(*str)) {
        ++str;
    }
    // Parse sign
    if(is_signed) {
        if(*str == '-') {
            coef = -1;
            ++str;
        }
    }
    if(*str == '+') {
        ++str;
    }
    // See if we need to parse base in C-like format
    // then set the base accordingly
    if(strpfx_i(str, "0X")) {
        ++str;
        if(base == 16 || base == 0) {
            ++str;
            base = 16;
        }
        else {
            goto finish;
        }
    }
    else if(*str == '0') {
        ++str;
        ++digits_read;
        if(base == 8 || base == 0) {
            base = 8;
        }
    }
    // Parse the string of digits in the given base. If the value
    // exceeds abs(int_min) we exit with range error.
    while(isbase(*str, (int)base)) {
        intull digit = (intull)todigit(*str);
        if(value > (int_abs_max - digit)/base) {
            goto error_out_of_range;
        }
        value = base*value + digit;
        ++str;
        ++digits_read;
    }
    // We only allow the modulo of value equal to abs(int_min) if it is
    // preceeded by the minus sign.
    if(is_signed) {
        if(value == int_abs_max && coef != -1) {
            goto error_out_of_range;
        }
    }
    goto finish;
    error_out_of_range:
    // Skip the remainder of the subject string
    while(isbase(*str, (int)base)) {
        ++str;
    }
    errno = ERANGE;
    value = int_max;
    goto finish;
    finish:;
    // If no conversion is performed we return the value of 0 and *endptr
    // is set to the nptr.
    bool conv_performed = (digits_read > 0);
    if(!conv_performed) {
        value = 0;
    }
    if(endptr != NULL) {
        if(!conv_performed) {
            *endptr = (char *)nptr;
        }
        else {
            *endptr = (char *)str;
        }
    }
    *coefptr = coef;
    return value;
}

intl strtol(const char *restrict nptr, char **restrict endptr, int base) {
    intull int_max = (intull)LONG_MAX;
    intl coef;
    intull modulo = strtoi_generic(nptr, endptr, base, &coef, int_max);
    intl value;
    if(modulo == int_max) {
        value = LONG_MIN;
    }
    else {
        value = coef * (intl)modulo;
    }
    return value;
}

intll strtoll(const char *restrict nptr, char **restrict endptr, int base) {
    intull int_max = (intull)LLONG_MAX;
    intl coef;
    intull modulo = strtoi_generic(nptr, endptr, base, &coef, int_max);
    intll value;
    if(modulo == int_max) {
        value = LLONG_MIN;
    }
    else {
        value = (intll)coef * (intll)modulo;
    }
    return value;
}

intul strtoul(const char *restrict nptr, char **restrict endptr, int base) {
    intull int_max = (intull)ULONG_MAX;
    intull value = strtoi_generic(nptr, endptr, base, NULL, int_max);
    return (intul)value;
}

intull strtoull(const char *restrict nptr, char **restrict endptr, int base) {
    intull int_max = (intull)ULLONG_MAX;
    return strtoi_generic(nptr, endptr, base, NULL, int_max);
}

int atoi(const char *nptr) {
    return (int)strtol(nptr, (char **)NULL, 10);
}

long int atol(const char *nptr) {
    return strtol(nptr, (char **)NULL, 10);
}

long long int atoll(const char *nptr) {
    return strtoll(nptr, (char **)NULL, 10);
}

char *itoa(int value, char *str, int base) {
    int sign = 0;
    if(value < 0) {
        sign = 1;
        value = -value;
    }
    char buf[20] = {0};
    char *bufp = buf + sizeof buf - 1;
    do {
        *--bufp = value%base+'0';
        value /= base;
    } while(value != 0);
    if(sign) {
        *--bufp = '-';
    }
    strcpy(str, bufp);
    return str;
}
