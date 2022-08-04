#pragma once

#include <stdint.h>

#if !defined(NULL)
    #define NULL ((void *)0)
#endif

#define TIME_UTC 1

#define CLOCKS_PER_SEC  ((clock_t)1000000)

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

clock_t    clock       (void);
time_t     time        (time_t *timer);
time_t     mktime      (struct tm *timeptr);
double     difftime    (time_t time1, time_t time0);
int        timespec_get(struct timespec *ts, int base);
char      *asctime     (const struct tm *timeptr);
char      *ctime       (const time_t *timer);
struct tm *gmtime      (const time_t *timer);
struct tm *localtime   (const time_t *timer);

size_t strftime(
    char *restrict s,
    size_t maxsize,
    const char *restrict format,
    const struct tm *restrict timeptr
);

#ifdef __STDC_WANT_LIB_EXT1__

errno_t asctime_s(char *s, rsize_t maxsize, const struct tm *timeptr);
errno_t ctime_s  (char *s, rsize_t maxsize, const time_t *timer);
struct tm *gmtime_s   (const time_t * restrict timer, struct tm * restrict result);
struct tm *localtime_s(const time_t * restrict timer, struct tm * restrict result);

#endif
