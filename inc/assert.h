
#pragma once

#if !defined(__func__)
    #define __func__ __FUNCTION__
#endif

void _Noreturn _assert(
    char const *cond,
    char const *func,
    char const *file,
    int line
);

#if defined(NDEBUG)
    #define assert(ignore) ((void)0)
#else
    #define _static_assert _Static_assert
    #define assert(condition)                                                 \
        do {                                                                  \
            if(!(condition)) {                                                \
                _assert(#condition, __func__, __FILE__, __LINE__);            \
            }                                                                 \
        } while(0)
#endif
