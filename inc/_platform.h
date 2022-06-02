#if !defined(_platform_h)
#define _platform_h

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

#if !(defined(_compiler_msvc) \
   || defined(_compiler_gnu) \
   || defined(_compiler_clang))
    #error "Unsupported Compiler"
#endif

#if !defined(_func)
    #if defined(_compiler_msvc)
        #define _func __FUNCTION__
    #else
        #define _func __func__
    #endif
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

#endif