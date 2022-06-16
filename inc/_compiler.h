
#pragma once

#if defined(_MSC_VER) && !defined(__clang__)
    #define _compiler_msvc
#endif

#if defined(__GNUC__) && !defined(__clang__)
    #define _compiler_gcc
#endif

#if defined(__clang__)
    #define _compiler_clang
#endif

#if defined(__CUIKC__)
    #define _compiler_cuik
#endif

#if !(defined(_compiler_msvc) \
   || defined(_compiler_gcc) \
   || defined(_compiler_cuik) \
   || defined(_compiler_clang))
    #error "Unsupported Compiler"
#endif

