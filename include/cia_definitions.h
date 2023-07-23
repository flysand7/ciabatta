
#pragma once

// Pre-C23 keyword macros and stddef
#define static_assert _Static_assert
#define NULL ((void *)0)

// Platform macros
#define CIA_LINUX 1
#define CIA_WINDOWS 2
#define CIA_ANDROID 3

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
   #define CIA_OS CIA_WINDOWS
   #if !defined(_WIN64)
      #error "32-bit windows is not supported"
   #endif
#elif __APPLE__
    #error "Apple OS's not supported and will never be unless you want to write the support for it"
#elif __ANDROID__
    #define CIA_OS CIA_ANDROID
#elif __linux__
    #define CIA_OS CIA_LINUX
#else
    #error "Unable to detect the OS"
#endif

// Convenience platform checking macros
#define os_is_linux() (CIA_OS == CIA_LINUX)
#define os_is_windows() (CIA_OS == CIA_WINDOWS)
#define os_is_android() (CIA_OS == CIA_ANDROID)

// Assert commonly-accepted platform-invariant sizes
static_assert(sizeof(char) == 1, "Char isn't 1 bytes long");
static_assert(sizeof(short) == 2, "Short isn't 2 bytes long");
static_assert(sizeof(int) == 4, "Int isn't 4 bytes long");
static_assert(sizeof(long long int) == 8, "Long long isn't 8 bytes long");
#if os_is_linux()
    static_assert(sizeof(long) == 8, "Long on linux isn't 8 bytes");
#elif os_is_windows()
    static_assert(sizeof(long) == 4, "Long on windows isn't 4 bytes");
#endif

// stdint.h type definitions
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
#if os_is_linux()
    typedef signed long int64_t;
    typedef unsigned long uint64_t;
#elif os_is_windows()
    typedef signed long long int64_t;
    typedef unsigned long long uint64_t;
#else
    #error "Platform not implemented"
#endif
    
// stdbool.h
typedef _Bool bool;
#define true ((bool)1)
#define false ((bool)0)
    
// Short type definitions
typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
