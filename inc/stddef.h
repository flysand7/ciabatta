
#pragma once

#define __bool_true_false_are_defined 1
#define __alignas_is_defined 1
#define __alignof_is_defined 1

#define NULL ((void *)0)
#define offsetof(st, m) ((size_t)((char *)&((st *)0)->m - (char *)0))

#if defined(_WIN32)
    typedef unsigned long size_t;
#else
    typedef unsigned long long size_t;
#endif

typedef long long          ptrdiff_t;
typedef long long          max_align_t;
typedef unsigned short     wchar_t; //

#define bool _Bool
#define true 1
#define false 0

#define alignas _Alignas
#define alignof _Alignof

#if !defined(_MSC_VER)
    #define noreturn _Noreturn
#else
    #define noreturn declspec(noreturn)
#endif
