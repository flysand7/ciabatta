#if !defined(_assert_h)
#define _assert_h

#include "_platform.h"

extern void _assert_print(char *cond, char const *func, char const *file, char const *line);

// TODO: use abort() to break
#if defined(_compiler_msvc)
    #include <intrin.h>
    #define _compiler_brk __debugbreak
#else
    #define _compiler_brk __builtin_trap
#endif

#define _str_(x) #x
#define _str(x) _str_(x)

#if defined(NDEBUG)
    #define assert(ignore) ((void)0)
#else
    #define _static_assert _Static_assert
    #define assert(condition)                                                 \
        do {                                                                  \
            if(!(condition)) {                                                \
                _assert_print(#condition, _func, __FILE__, _str(__LINE__));   \
                _compiler_brk();                                              \
            }                                                                 \
        } while(0)
#endif

#endif