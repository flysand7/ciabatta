
#pragma once

#if defined(_MSC_VER) && !defined(__clang__)
    #define _compiler_msvc
#endif

#if defined(__GNUC__) && !defined(__clang__)
    #define _compiler_gnu
#endif

#if defined(__clang__)
    #define _compiler_clang
#endif

#if defined(__CUIKC__)
    #define _compiler_cuik
#endif

#if !(defined(_compiler_msvc) \
   || defined(_compiler_gnu) \
   || defined(_compiler_cuik) \
   || defined(_compiler_clang))
    #error "Unsupported Compiler"
#endif

#if defined(_compiler_msvc) || defined(_compiler_cuik)
    #include <intrin.h>
    #define _compiler_brk __debugbreak
#elif defined(_compiler_gcc) || defined(_compiler_clang)
    #define _compiler_brk __builtin_trap
#else
    #error "_compiler_brk is not implemented for this compiler"
#endif
