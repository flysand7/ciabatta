
#pragma once

typedef struct mbstate_t mbstate_t;
struct mbstate_t {
    union {
        unsigned short leftover;
        unsigned short high_surrogate;
    };
};

