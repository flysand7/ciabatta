
#define asuint64(x) ((union {double f; uint64_t i;}){x}).i
#define asdouble(x) ((union {double f; uint64_t i;}){x}).f

#include <_compiler.h>
#if defined(_compiler_clang) || defined(_compiler_gnu)
    #define just_do_it(t) __attribute__((unused)) volatile t
#else
    #define just_do_it(t) volatile t
#endif
