#pragma once

#include "_compiler.h"
#include "_macros.h"

extern void _assert_error(
    char *cond,
    char const *func,
    char const *file,
    char const *line
);

#if defined(NDEBUG)
    #define assert(ignore) ((void)0)
#else
    #define _static_assert _Static_assert
    #define assert(condition)                                                 \
        do {                                                                  \
            if(!(condition)) {                                                \
                _assert_error(                                                \
                    #condition,                                               \
                    _compiler_curfunc,                                        \
                    __FILE__,                                                 \
                    _str(__LINE__));                                          \
            }                                                                 \
        } while(0)
#endif
