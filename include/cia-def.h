
#pragma once

// Since we're re-defining noreturn below, this would mess
// with __declspec(noreturn) in windows headers, which
// is hidden behind a define. Good thing, because now we
// can override that define over here.
#if defined(_CIA_OS_WINDOWS)
    #define DECLSPEC_NORETURN __declspec("noreturn")
#endif

// Pre-C23 keyword macros and stddef
#define static_assert _Static_assert
#define NULL ((void *)0)

// Assert commonly-accepted platform-invariant sizes
static_assert(sizeof(char) == 1, "Char isn't 1 bytes long");
static_assert(sizeof(short) == 2, "Short isn't 2 bytes long");
static_assert(sizeof(int) == 4, "Int isn't 4 bytes long");
static_assert(sizeof(long long int) == 8, "Long long isn't 8 bytes long");
#if defined(_CIA_OS_LINUX)
    static_assert(sizeof(long) == 8, "Long on linux isn't 8 bytes");
#elif defined(_CIA_OS_WINDOWS)
    static_assert(sizeof(long) == 4, "Long on windows isn't 4 bytes");
#endif

// stdint.h type definitions
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;
#if defined(_CIA_OS_LINUX)
    typedef signed long int64_t;
    typedef unsigned long uint64_t;
#elif defined(_CIA_OS_WINDOWS)
    typedef signed long long int64_t;
    typedef unsigned long long uint64_t;
#else
    #error "Platform not implemented"
#endif

// stdbool.h
#if !defined(__bool_true_false_are_defined)
    typedef _Bool bool;
    #define true ((bool)1)
    #define false ((bool)0)
#endif
    
// Short type definitions
typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
