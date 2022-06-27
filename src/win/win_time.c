
#include <time.h>
#include <stdint.h>
#include <win.h>

#define TIME_TICKS_PER_SEC          1000
#define FILE_TICKS_PER_SEC          10000000
#define FILE_TICKS_UNTIL_UNIX_EPOCH 116444736000000000ULL

#define NANOS_PER_SEC               1000000000
#define NANOS_PER_FILE_TICK         (NANOS_PER_SEC / FILE_TICKS_PER_SEC)
#define NANOS_PER_TIME_TICK         (NANOS_PER_SEC / TIME_TICKS_PER_SEC)


// Store the time since we started running the process
static uint64_t timer_freq;
static uint64_t timer_start;

void _setup_timer(void) {
    LARGE_INTEGER freq, start;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    timer_start = start.QuadPart;
    timer_freq  = freq.QuadPart;
}

static ULONGLONG win32_get_unix_nanos() {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    ULARGE_INTEGER ft64 = {
        .LowPart  = ft.dwLowDateTime,
        .HighPart = ft.dwHighDateTime,
    };
    ULONGLONG fticks      = ft64.QuadPart;
    ULONGLONG unix_fticks = fticks - FILE_TICKS_UNTIL_UNIX_EPOCH;
    ULONGLONG unix_nanos  = unix_fticks * NANOS_PER_FILE_TICK;
    return unix_nanos;
}

clock_t clock(void) {
    LARGE_INTEGER curr;
    if (!QueryPerformanceCounter(&curr)) {
        return -1;
    }
    if (curr.QuadPart < timer_start) {
        return -1;
    }
    clock_t ticks = curr.QuadPart - timer_start;
    // Basically millis / timer_freq * CLOCKS_PER_SEC but more precise
    return ticks / timer_freq * CLOCKS_PER_SEC
         + ticks % timer_freq * CLOCKS_PER_SEC / timer_freq;
}

time_t time(time_t *timer) {
    ULONGLONG unix_nanos  = win32_get_unix_nanos();
    time_t    timer_ticks = unix_nanos / NANOS_PER_TIME_TICK;
    if(timer != NULL) {
        *timer = timer_ticks;
    }
    return timer_ticks;
}

double difftime(time_t time1, time_t time0) {
    double resolution = (double)TIME_TICKS_PER_SEC;
    if(time1 >= time0) return  (double)(time1 - time0) / resolution;
    else               return -(double)(time0 - time1) / resolution;
}

int timespec_get(struct timespec *ts, int base) {
    if (base != TIME_UTC) return 0;
    ULONGLONG unix_nanos = win32_get_unix_nanos();
    ts->tv_sec  = unix_nanos / NANOS_PER_SEC;
    ts->tv_nsec = unix_nanos;
    return base;
}
