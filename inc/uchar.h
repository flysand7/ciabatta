
#pragma once

#include <stddef.h>
#include <stdint.h>

#if !defined(_mbstate_t_defined)
    #define _mbstate_t_defined
    typedef struct mbstate_t mbstate_t;
    struct mbstate_t {
        union {
            unsigned short leftover;
            unsigned short high_surrogate;
        };
    };
#endif

typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;

size_t mbrtoc16(
    char16_t   *restrict pc16,
    char const *restrict s,
    size_t               n,
    mbstate_t  *restrict ps
);

size_t c16rtomb(
    char *restrict      s,
    char16_t            c16,
    mbstate_t *restrict ps
);

size_t mbrtoc32(
    char32_t   *restrict pc32,
    char const *restrict s,
    size_t               n,
    mbstate_t  *restrict ps
);

size_t c32rtomb(
    char *restrict      s,
    char32_t            c32,
    mbstate_t *restrict ps
);
