#pragma once

#include <stdint.h>

#if !defined(NULL)
#define NULL ((void *)0)
#endif

#define TIME_UTC 1

#define CLOCKS_PER_SEC  ((clock_t)1000000)

typedef uint64_t clock_t;
typedef uint64_t time_t;

struct timespec {
	time_t tv_sec;
	long   tv_nsec;
};

struct tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mon;
    int tm_mday; // Days passed within 1st of current month
    int tm_year; // Years since year 1900
    int tm_wday; // Days passed since sunday
    int tm_yday; // Days passed since Jan 1
    int tm_isdst;
};

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
errno_t ctime_s(char *s, rsize_t maxsize, const time_t *timer);
struct tm *gmtime_s(const time_t * restrict timer, struct tm * restrict result);
struct tm *localtime_s(const time_t * restrict timer, struct tm * restrict result);
#endif
