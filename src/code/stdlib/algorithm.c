#include <stdlib.h>

int abs(int n) {
    if(n < 0) return -n;
    return n;
}

long absl(long n) {
    if(n < 0) return -n;
    return n;
}

long long absll(long long n) {
    if(n < 0) return -n;
    return n;
}

div_t div(int x, int y) {
    div_t res;
    res.quot = x / y;
    res.rem = x % y;
    return res;
}

ldiv_t ldiv(long x, long y) {
    ldiv_t res;
    res.quot = x / y;
    res.rem = x % y;
    return res;
}

lldiv_t lldiv(long long x, long long y) {
    lldiv_t res;
    res.quot = x / y;
    res.rem = x % y;
    return res;
}

const void *bsearch(
    const void *key,
    const void *base,
    size_t nmemb,
    size_t size,
    int (*compar)(const void *, const void *)
) {
    size_t left = 0;
    size_t right = nmemb;

    const char* buffer = base;
    while (left < right) {
        size_t middle = (left + right) / 2;

        int cmp = compar(&buffer[middle * size], key);
        if (cmp == 0) return &buffer[left * size];
        if (cmp < 0) left = middle + 1;
        else right = middle;
    }

    return NULL;
}

void qsort(
    void *base,
    size_t nmemb,
    size_t size,
    int (*diff)(const void *, const void *)
) {
    // Ima be doing bublbe sort for now
    char *bytes = base;
    for(size_t i = 0; i != nmemb-1; ++i) {
        for(size_t j = 0; j < nmemb-i-1; ++j) {
            char *this = bytes+j*size;
            char *that = this+size;
            if(diff(this, that) > 0) {
                for(size_t b=0;b!=size;++b) {
                    char temp = this[b];
                    this[b] = that[b];
                    that[b] = temp;
                }
            }
        }
    }
}
