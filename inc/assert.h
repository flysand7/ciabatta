#pragma once

#if !defined(__func__)
    #define __func__ __FUNCTION__
#endif

#define _assert_str_(x) #x
#define _assert_str(x) _assert_str_(x)

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
                    __func__,                                                 \
                    __FILE__,                                                 \
                    _assert_str(__LINE__));                                   \
            }                                                                 \
        } while(0)
#endif
