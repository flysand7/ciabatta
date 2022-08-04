
#pragma once

#define __bool_true_false_are_defined 1
#define __alignas_is_defined 1
#define __alignof_is_defined 1

#if !defined(NULL)
    #define NULL ((void *)0)
#endif

#if !defined(bool)
    #define bool _Bool
#endif

#if !defined(true)
    #define true 1
#endif

#if !defined(false)
    #define false 0
#endif

#define offsetof(st, m) ((size_t)((char *)&((st *)0)->m - (char *)0))
#define alignas _Alignas
#define alignof _Alignof

#if !defined(_MSC_VER)
    #define noreturn _Noreturn
#else
    #define noreturn declspec(noreturn)
#endif

#if defined(_WIN32)
    typedef long long ptrdiff_t;
    typedef unsigned long long size_t;
    typedef unsigned long long max_align_t;
    typedef unsigned short wchar_t;
#else
    typedef long ptrdiff_t;
    typedef unsigned long size_t;
    typedef unsigned long max_align_t;
    typedef int wchar_t;
#endif

