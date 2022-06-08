#include <stdlib.h>

const void *bsearch(const void *key, const void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
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
