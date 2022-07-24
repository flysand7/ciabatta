
#pragma once

typedef struct mbstate_t mbstate_t;
struct mbstate_t {
    union {
        char16_t leftover;
        char16_t high_surrogate;
    };
};

