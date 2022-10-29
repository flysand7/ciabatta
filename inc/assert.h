
#pragma once

#if defined(_MSC_VER) && !defined(__clang__)
    #define _Noreturn
#endif

void _assert(
    char const *cond,
    char const *func,
    char const *file,
    int line
);

#if defined(NDEBUG)
    #define assert(ignore) ((void)0)
#elif defined(_DEBUG)
    #if defined(__GNUC__) || defined(__CUIKC__)
        #define assert(c) if (!(c)) __builtin_trap()
    #elif defined(_MSC_VER)
        #define assert(c) if (!(c)) __debugbreak()
    #else
        // In debug mode there shouldn't be any optimizations so this should
        // work as a simple way to cause a trap.
        #define assert(c) do { if (!(c)) *(volatile int *)0 = 0; } while(0)
    #endif
#else
    #define _static_assert _Static_assert
    #define assert(condition)                                                 \
        do {                                                                  \
            if(!(condition)) {                                                \
                _assert(#condition, __func__, __FILE__, __LINE__);            \
            }                                                                 \
        } while(0)
#endif
