
#pragma once

#if !defined(__func__)
    #define __func__ __FUNCTION__
#endif

#if defined (_MSC_VER)
    #define _Noreturn
#endif

void _Noreturn _assert(
    char const *cond,
    char const *func,
    char const *file,
    int line
);

#if defined(NDEBUG)
    #define assert(ignore) ((void)0)
#elif defined(DEBUG)
    #if __GNUC__
        #define assert(c) if (!(c)) __builtin_trap()
    #elif _MSC_VER
        #define assert(c) if (!(c)) __debugbreak()
    // TODO: Handle Cuik, and if such handling is not required remove this comment
    #else
        // In debug mode there shouldn't be any optimizations so this should
        // work as a simple way to cause a trap.
        #define assert(c) if (!(c)) *(volatile int *)0 = 0
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
