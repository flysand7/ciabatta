
#pragma once

#if !defined(NULL)
    #define NULL ((void *)0)
#endif

// The number of clock ticks per second
#define CLOCKS_PER_SEC  ((clock_t)1000)
#define TIME_UTC 1

struct timespec {
	time_t tv_sec;  // Seconds - >= 0
	long   tv_nsec; // Nanoseconds - [0, 999999999]
};

struct tm {
    int tm_sec;   // seconds after the minute - [0, 60] including leap second
    int tm_min;   // minutes after the hour - [0, 59]
    int tm_hour;  // hours since midnight - [0, 23]
    int tm_mday;  // day of the month - [1, 31]
    int tm_mon;   // months since January - [0, 11]
    int tm_year;  // years since 1900
    int tm_wday;  // days since Sunday - [0, 6]
    int tm_yday;  // days since January 1 - [0, 365]
    int tm_isdst; // daylight savings time flag
};

clock_t clock(void);
double difftime(time_t time1, time_t time0);
time_t mktime(struct tm *timeptr);
time_t time(time_t *timer);
int timespec_get(timespec *ts, int base);
char *asctime(const struct tm *timeptr);
char *ctime(const time_t *timer);
struct tm *gmtime(const time_t *timer);
struct tm *localtime(const time_t *timer);
size_t strftime(char * restrict s, size_t maxsize, const char * restrict format, const struct tm * restrict timeptr);

#ifdef __STDC_WANT_LIB_EXT1__
	errno_t asctime_s(char *s, rsize_t maxsize, const struct tm *timeptr);
	errno_t ctime_s(char *s, rsize_t maxsize, const time_t *timer);
	struct tm *gmtime_s(const time_t * restrict timer, struct tm * restrict result);
	struct tm *localtime_s(const time_t * restrict timer, struct tm * restrict result);
#endif
