
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>

// TODO: strto*: locale-based parsing for integers
// TODO: i made a function that parses longs and i'm fucken
// tired. someone make float parsing :kekw:

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

long
strtol(const char *restrict nptr, char **restrict endptr, int inbase) {
    if(!inrange(0, inbase, 36)) {
        *endptr = NULL;
        return 0;
    }
    // Skip space on the beginning
    while(isspace(*nptr)) {
        ++nptr;
    }
    // Parse sign
    long coef = 1;
    if(*nptr == '-') {
        coef = -1;
        ++nptr;
    }
    if(*nptr == '+') {
        ++nptr;
    }
    unsigned long base = (unsigned long)inbase;
    unsigned long value = 0;
    // See if we need to parse base in C-like format
    if(*nptr == '0' && *(nptr+1) == 'x') {
        ++nptr;
        if(base == 16 || base == 0) {
            ++nptr;
            base = 16;
        }
        else {
            value = 0;
            goto end;
        }
    }
    else if(*nptr == '0') {
        ++nptr;
        if(base == 8 || base == 0) {
            base = 8;
        }
    }
    while(isbase(*nptr, (int)base)) {
        unsigned long digit = (unsigned long)todigitl(*nptr);
        if(value > (ULONG_MAX - digit)/base) {
            errno = ERANGE;
            value = 0;
            goto end;
        }
        value = base*value + digit;
        ++nptr;
    }
    unsigned long max_modulo = (unsigned long)LONG_MAX+1;
    if(value > max_modulo) {
        errno = ERANGE;
        value = 0;
        goto end;
    }
    if(value == max_modulo) {
        if(coef == 1) {
            errno = ERANGE;
            value = 0;
            goto end;
        }
        else {
            value = LONG_MIN;
            coef = 1;
        }
    }
end:
    if(endptr != NULL) {
        *endptr = (char *)nptr;
    }
    return coef*(long)value;
}

long long
strtoll(const char *restrict nptr, char **restrict endptr, int base) {
    return 0;
}

unsigned long
strtoul(const char *restrict nptr, char **restrict endptr, int base) {
    return 0;
}

unsigned long long
strtoull(const char *restrict nptr, char **restrict endptr, int base) {
    return 0;
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
