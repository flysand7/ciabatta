
#include <stdlib.h>

int mblen(const char *s, size_t n) {
    return mbtowc((wchar_t*)0, s, n);
}

int mbtowc(wchar_t *restrict pwc, const char *restrict s, size_t n) {
    return 0;
}

size_t mbstowcs(wchar_t *restrict pwcs, const char *restrict s, size_t n) {
    return 0;
}

int wctomb(char *s, wchar_t wchar) {
    return 0;
}

size_t wcstombs(char *restrict s, const wchar_t *restrict pwcs, size_t n) {
    return 0;
}

