
#pragma once

#if !defined(NULL)
    #define NULL ((void *)0)
#endif

#define offsetof(st, m) ((size_t)((char *)&((st *)0)->m - (char *)0))

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

#if __STDC_WANT_LIB_EXT1__ == 1
    typedef size_t rsize_t;
#endif
