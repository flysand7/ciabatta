#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

// Microsoft uses this to refer to the secure functions so we'll allow it
#ifdef __STDC_WANT_SECURE_LIB__
    #define __STDC_WANT_LIB_EXT1__ 1
#endif

// Compiler Identification

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

// OS Identification

#if defined(_WIN32)
    #define _os_win
#endif

#if defined(__linux__) && !defined(__ANDROID__)
    #define _os_linux
#endif

#if !(defined(_os_win) \
    || defined(_os_linux))
    #error "Unsupported OS"
#endif

#ifdef __STDC_WANT_LIB_EXT1__
    typedef int errno_t;
    typedef size_t rsize_t;
#endif

