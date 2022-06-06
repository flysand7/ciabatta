
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <math.h>

// TODO: strto*: locale-based parsing
// TODO: correctly rounded base 16 floats parsing

// Call me weak if you want but I'm actually too lazy to type
// them out every time, also they take up a lot of horiz space.
typedef long long intll;
typedef long intl;
typedef unsigned long long intull;
typedef unsigned long intul;
typedef unsigned intu;

#define inrange(start, c, end) ((start) <= (c) && (c) <= (end))

static bool isbase(int c, int base) {
    int val = 0;
    if(isdigit(c)) {
        val = c-'0';
    }
    else if(islower(c)) {
        val = c-'a'+10;
    }
    else if(isupper(c)) {
        val = c-'A'+10;
    }
    else {
        return false;
    }
    return val < base;
}

static bool strprefix_i(char const *restrict str, char const *restrict prefix) {
    while(*prefix != 0) {
        if(*str == 0) break;
        if(toupper(*str) != toupper(*prefix)) return false;
        ++prefix;
        ++str;
    }
    return true;
}

// Called only when isbase(c, base) for some base in range
static long todigit(int c) {
    int val = 0;
    if(isdigit(c)) {
        val = c-'0';
    }
    else if(islower(c)) {
        val = c-'a'+10;
    }
    else if(isupper(c)) {
        val = c-'A'+10;
    }
    return val;
}

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
    if(!inrange(0, inbase, 36)) {
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
    if(strprefix_i(str, "0X")) {
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
    finish:
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

static double strtod_generic(const char *restrict nptr, char **restrict endptr) {
    const char *restrict str = nptr;
    bool conv_performed = false;
    double value = 0.0;
    double coef = 1.0;
    // Skip space on the beginning
    while(isspace(*str)) {
        ++str;
    }
    // Check for inf and nan
    if(strprefix_i(str, "INF")) {
        str += sizeof "INF"-1;
        value = HUGE_VAL;
        conv_performed = true;
        goto finish;
    }
    if(strprefix_i(str, "INFINITY")) {
        str += sizeof "INFINITY"-1;
        value = HUGE_VAL;
        conv_performed = true;
        goto finish;
    }
    if(strprefix_i(str, "NAN")) {
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
    if(strprefix_i(str, "0X")) {
        str += sizeof "0X"-1;
        base = 16;
    }
    // Parse the whole part
    while(isbase(*str, base)) {
        long digit = todigit(*str);
        value = 10.0*value + (double)digit;
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
        double fract = (double)digit;
        long cexp = exp;
        while(cexp-- != 0) {
            fract /= (double)base;
        }
        value += fract;
        ++exp;
        ++str;
    }
    // Parse the exponent
    if((base == 10 && strprefix_i(str, "E"))
       || (base == 16 && strprefix_i(str, "P")))
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

double atof(const char *nptr) {
    return strtod(nptr, (char **)NULL);
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

double strtod(const char *restrict nptr, char **restrict endptr) {
    return strtod_generic(nptr, endptr);
}

float strtof(const char *restrict nptr, char **restrict endptr) {
    return (float)strtod_generic(nptr, endptr);
}

long double strtold(const char *restrict nptr, char **restrict endptr) {
    return (long double)strtod_generic(nptr, endptr);
}
