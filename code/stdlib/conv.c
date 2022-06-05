
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>

// TODO: strto*: locale-based parsing for integers
// TODO: i made a function that parses longs and i'm fucken
// tired. someone make float parsing :kekw:

// Call me weak if you want but I'm actually too lazy to type
// them out every time, also they take up a lot of horiz space.
typedef long long int          intll;
typedef long int               intl;
typedef unsigned long long int intull;
typedef unsigned long int      intul;
typedef unsigned int           intu;

#define inrange(start, c, end) ((start) <= (c) && (c) <= (end))

static bool isbase(int c, int base) {
    int val;
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

// Called only when isbase(c, base) for some base in range
static long todigitl(int c) {
    int val;
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

static intull strto_generic(
    const char *restrict nptr,
    char **restrict endptr,
    int inbase,
    intl *coefptr,
    intull int_max
) {
    const char *restrict str = nptr;
    intull value = 0;
    int digits_read = 0;
    bool is_signed = (coefptr != NULL);
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
    intl coef = 1;
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
    if(*str == '0' && (*(str+1) == 'x' || *(str+1) == 'X')) {
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
        intull digit = (intull)todigitl(*str);
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

intl strtol(const char *restrict nptr, char **restrict endptr, int base) {
    intull int_max = (intull)LONG_MAX;
    intl coef;
    intull modulo = strto_generic(nptr, endptr, base, &coef, int_max);
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
    intull modulo = strto_generic(nptr, endptr, base, &coef, int_max);
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
    intull value = strto_generic(nptr, endptr, base, NULL, int_max);
    return (intul)value;
}

intull strtoull(const char *restrict nptr, char **restrict endptr, int base) {
    intull int_max = (intull)ULLONG_MAX;
    return strto_generic(nptr, endptr, base, NULL, int_max);
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
    return 0;
}

float strtof(const char *restrict nptr, char **restrict endptr) {
    return 0;
}

long double strtold(const char *restrict nptr, char **restrict endptr) {
    return 0;
}
