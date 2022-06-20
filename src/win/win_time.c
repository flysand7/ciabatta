
#include <time.h>
#include <stdint.h>
#include <win.h>

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

int timespec_get(struct timespec *ts, int base) {
    if (base != TIME_UTC) return 0;

    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    ULARGE_INTEGER s = {
        .LowPart = ft.dwLowDateTime,
        .HighPart = ft.dwHighDateTime
    };

    ULONGLONG t = s.QuadPart - 116444736000000000ULL;
    ts->tv_sec = t / 10000000;
    ts->tv_nsec = ((int) (t % 10000000)) * 100;
    return base;
}

clock_t clock(void) {
    LARGE_INTEGER curr;
    if (!QueryPerformanceCounter(&curr)) {
        return -1;
    }

    // time travel?
    if (curr.QuadPart < timer_start) {
        return -1;
    }

    uint64_t time_from_start = curr.QuadPart - timer_start;

    uint64_t scaled_millis = (time_from_start / timer_freq) * CLOCKS_PER_SEC;
    // helps reduce error when doing integer division
    scaled_millis += ((time_from_start % timer_freq) * CLOCKS_PER_SEC) / timer_freq;

    return scaled_millis;
}
