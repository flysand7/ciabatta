#pragma once

// 7.29.1 p.2: Macros

#define __STDC_VERSION_TIME_H__ 202311L

#if !defined(NULL)
    #define NULL ((void *)0)
#endif

#define CLOCKS_PER_SEC  ((clock_t)1000000)

#define TIME_UTC 1
#define TIME_MONOTONIC 2
#define TIME_ACTIVE 3
#define TIME_THREAD_ACTIVE 4

// 7.29.1 p.4: Types

#if defined(_WIN32)
    typedef unsigned long long size_t;
#else
    typedef unsigned long size_t;
#endif

typedef unsigned long long clock_t;
typedef unsigned long long time_t;

#if !defined(_timespec_defined)
    #define _timespec_defined
    struct timespec {
        time_t tv_sec;
        long   tv_nsec;
    };
#endif

#if !defined(_tm_defined)
    #define _tm_defined
    struct tm {
        int tm_sec;
        int tm_min;
        int tm_hour;
        int tm_mon;
        int tm_mday;
        int tm_year;
        int tm_wday;
        int tm_yday;
        int tm_isdst;
    };
#endif

// Basic timer functions
clock_t clock(void);
time_t time(time_t *timer);
double difftime(time_t time1, time_t time0);

// Human-readable time functions
time_t mktime(struct tm *timeptr);
struct tm *gmtime(const time_t *timer);
struct tm *localtime(const time_t *timer);

// Timespec functions
int timespec_get(struct timespec *ts, int base);
int timespec_getres(struct timespec *ts, int base);

// Time formatting functions
char *asctime(const struct tm *timeptr);
char *ctime(const time_t *timer);
size_t strftime(char *restrict str, size_t sz, const char *restrict fmt, const struct tm *restrict time);

#ifdef __STDC_WANT_LIB_EXT1__

errno_t asctime_s(char *s, rsize_t maxsize, const struct tm *timeptr);
errno_t ctime_s(char *s, rsize_t maxsize, const time_t *timer);
struct tm *gmtime_s(const time_t * restrict timer, struct tm * restrict result);
struct tm *localtime_s(const time_t * restrict timer, struct tm * restrict result);

#endif
