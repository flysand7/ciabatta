
#pragma once

static_assert(sizeof(char) == 1, "Char isn't 1 bytes long");
static_assert(sizeof(short) == 2, "Short isn't 2 bytes long");
static_assert(sizeof(int) == 4, "Int isn't 4 bytes long");
static_assert(sizeof(long long int) == 8, "Long long isn't 8 bytes long");
#if defined(_CIA_OS_LINUX)
    static_assert(sizeof(long) == 8, "Long on linux isn't 8 bytes");
#elif defined(_CIA_OS_WINDOWS)
    static_assert(sizeof(long) == 4, "Long on windows isn't 4 bytes");
#endif

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

typedef int8_t int_fast8_t, int_least8_t;
typedef int16_t int_fast16_t, int_least16_t;
typedef int32_t int_fast32_t, int_least32_t;
typedef int64_t int_fast64_t, int_least64_t;
typedef uint8_t uint_fast8_t, uint_least8_t;
typedef uint16_t uint_fast16_t, uint_least16_t;
typedef uint32_t uint_fast32_t, uint_least32_t;
typedef uint64_t uint_fast64_t, uint_least64_t;

// TODO: verify what size it is (on windows and linux)
typedef int64_t  intmax_t;
typedef int64_t  intptr_t;
typedef uint64_t uintmax_t;
typedef uint64_t uintptr_t;

// Constant expandors
#define INT8_C(n)    (n)
#define INT16_C(n)   (n)
#define INT32_C(n)   (n)
#define UINT8_C(n)   (n)
#define UINT16_C(n)  (n)
#define UINT32_C(n)  (n)

#if defined(_CIA_OS_LINUX)
    #define INT64_C(n)   (n ## L)
    #define INTMAX_C(n)  (n ## L)
    #define UINT64_C(n)  (n ## UL)
    #define UINTMAX_C(n) (n ## UL)
#elif defined(_CIA_OS_WINDOWS)
    #define INT64_C(n)   (n ## LL)
    #define INTMAX_C(n)  (n ## LL)
    #define UINT64_C(n)  (n ## ULL)
    #define UINTMAX_C(n) (n ## ULL)
#else
    #error "Platform functionality not defined"
#endif

#define INT8_WIDTH          8
#define INT16_WIDTH        16
#define INT32_WIDTH        32
#define INT64_WIDTH        64
#define UINT8_WIDTH         8
#define UINT16_WIDTH       16
#define UINT32_WIDTH       32
#define UINT64_WIDTH       64
#define INT_FAST8_WIDTH     8
#define INT_FAST16_WIDTH   16
#define INT_FAST32_WIDTH   32
#define INT_FAST64_WIDTH   64
#define UINT_FAST8_WIDTH    8
#define UINT_FAST16_WIDTH  16
#define UINT_FAST32_WIDTH  32
#define UINT_FAST64_WIDTH  64
#define INT_LEAST8_WIDTH    8
#define INT_LEAST16_WIDTH  16
#define INT_LEAST32_WIDTH  32
#define INT_LEAST64_WIDTH  64
#define UINT_LEAST8_WIDTH   8
#define UINT_LEAST16_WIDTH 16
#define UINT_LEAST32_WIDTH 32
#define UINT_LEAST64_WIDTH 64
#define INTPTR_WIDTH       64
#define INTMAX_WIDTH       64
#define UINTPTR_WIDTH      64
#define UINTMAX_WIDTH      64

// Temporary defines
#define _s8_min         -0x80
#define _s8_max          0x7f
#define _u8_max          0xff
#define _s16_min      -0x8000
#define _s16_max       0x7fff
#define _u16_max       0xffff
#define _s32_min  -0x80000000
#define _s32_max   0x7fffffff
#define _u32_max   0xffffffff
#define _s64_min INT64_C(-0x8000000000000000)
#define _s64_max INT64_C(0x7fffffffffffffff)
#define _u64_max UINT64_C(0xffffffffffffffff)

// Min/max for all integer types
#define INT8_MIN         _s8_min
#define INT_FAST8_MIN    _s8_min
#define INT_LEAST8_MIN   _s8_min
#define INT16_MIN        _s16_min
#define INT_FAST16_MIN   _s16_min
#define INT_LEAST16_MIN  _s16_min
#define INT32_MIN        _s32_min
#define INT_FAST32_MIN   _s32_min
#define INT_LEAST32_MIN  _s32_min
#define INT64_MIN        _s64_min
#define INT_FAST64_MIN   _s64_min
#define INT_LEAST64_MIN  _s64_min
#define INTPTR_MIN       _s64_min
#define INTMAX_MIN       _s64_min

#define INT8_MAX         _s8_max
#define INT_FAST8_MAX    _s8_max
#define INT_LEAST8_MAX   _s8_max
#define INT16_MAX        _s16_max
#define INT_FAST16_MAX   _s16_max
#define INT_LEAST16_MAX  _s16_max
#define INT32_MAX        _s32_max
#define INT_FAST32_MAX   _s32_max
#define INT_LEAST32_MAX  _s32_max
#define INT64_MAX        _s64_max
#define INT_FAST64_MAX   _s64_max
#define INT_LEAST64_MAX  _s64_max
#define INTPTR_MAX       _s64_max
#define INTMAX_MAX       _s64_max

#define UINT8_MAX        _u8_max
#define UINT_FAST8_MAX   _u8_max
#define UINT_LEAST8_MAX  _u8_max
#define UINT16_MAX       _u16_max
#define UINT_FAST16_MAX  _u16_max
#define UINT_LEAST16_MAX _u16_max
#define UINT32_MAX       _u32_max
#define UINT_FAST32_MAX  _u32_max
#define UINT_LEAST32_MAX _u32_max
#define UINT64_MAX       _u64_max
#define UINT_FAST64_MAX  _u64_max
#define UINT_LEAST64_MAX _u64_max
#define UINTPTR_MAX      _u64_max
#define UINTMAX_MAX      _u64_max

// Undo temporary defines
#undef _s8_max
#undef _s8_min
#undef _u8_max
#undef _s16_max
#undef _s16_min
#undef _u16_max
#undef _s32_max
#undef _s32_min
#undef _u32_max
#undef _s64_max
#undef _s64_min
#undef _u64_max