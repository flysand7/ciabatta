
#define NS_PER_SEC                  1000000000
#define NS_PER_MS                   1000000

#define TIME_TICKS_PER_SEC          1000000000ULL
#define NS_BEFORE_UNIX_EPOCH        11644473600000000000ULL

typedef struct tm tm_t;

// Store the time since we started running the process
static uint64_t timer_freq;
static uint64_t timer_start;

// Initialize timers

static void _setup_timer(void) {
    LARGE_INTEGER freq, start;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    timer_start = start.QuadPart;
    timer_freq  = freq.QuadPart;
}

// Timestamp utilitity functions

static ULONGLONG _time_filetime_to_ns(FILETIME ft) {
    ULARGE_INTEGER ft64 = {
        .LowPart  = ft.dwLowDateTime,
        .HighPart = ft.dwHighDateTime,
    };
    ULONGLONG fticks = ft64.QuadPart;
    return fticks * 100;
}

static ULONGLONG _time_utc_ns() {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    ULONGLONG ns = _time_filetime_to_ns(ft);
    ULONGLONG unix_ns = ns - NS_BEFORE_UNIX_EPOCH;
    return unix_ns;
}

static ULONGLONG _time_mono_ns() {
    ULONGLONG time_ms = GetTickCount64();
    ULONGLONG time_ns = time_ms * NS_PER_MS;
    return time_ns;
}

static ULONGLONG _time_process_ns() {
    FILETIME _1;
    FILETIME _2;
    FILETIME ktime;
    FILETIME utime;
    HANDLE current_process = GetCurrentProcess();
    if(GetProcessTimes(current_process, &_1, &_2, &ktime, &utime) == 0) {
        abort();
    }
    ULONGLONG kernel_time = _time_filetime_to_ns(ktime);
    ULONGLONG user_time = _time_filetime_to_ns(utime);
    ULONGLONG cpu_time = kernel_time + user_time;
    return cpu_time;
}

static ULONGLONG _time_thread_ns() {
    FILETIME _1;
    FILETIME _2;
    FILETIME ktime;
    FILETIME utime;
    HANDLE current_thread = GetCurrentThread();
    if(GetThreadTimes(current_thread, &_1, &_2, &ktime, &utime) == 0) {
        abort();
    }
    ULONGLONG kernel_time = _time_filetime_to_ns(ktime);
    ULONGLONG user_time = _time_filetime_to_ns(utime);
    ULONGLONG cpu_time = kernel_time + user_time;
    return cpu_time;
}

// Timer functions

clock_t clock(void) {
    LARGE_INTEGER curr;
    if (!QueryPerformanceCounter(&curr)) {
        return -1;
    }
    if (curr.QuadPart < timer_start) {
        return -1;
    }
    clock_t ticks = curr.QuadPart - timer_start;
    // Same as (ticks / timer_freq * CLOCKS_PER_SEC) but more precise
    return ticks / timer_freq * CLOCKS_PER_SEC
         + ticks % timer_freq * CLOCKS_PER_SEC / timer_freq;
}

time_t time(time_t *timer) {
    ULONGLONG unix_nanos  = _time_mono_ns();
    time_t    timer_ticks = unix_nanos;
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
    ULONGLONG current_ns;
    switch(base) {
        case TIME_UTC:           current_ns = _time_utc_ns(); break;
        case TIME_MONOTONIC:     current_ns = _time_mono_ns(); break;
        case TIME_ACTIVE:        current_ns = _time_process_ns(); break;
        case TIME_THREAD_ACTIVE: current_ns = _time_thread_ns(); break;
        default: return 0;
    }
    ts->tv_sec  = current_ns / NS_PER_SEC;
    ts->tv_nsec = current_ns;
    return base;
}

int timespec_getres(struct timespec *ts, int base) {
    switch(base) {
        case TIME_UTC:           return NS_PER_SEC;
        case TIME_MONOTONIC:     return NS_PER_SEC;
        case TIME_ACTIVE:        return NS_PER_SEC;
        case TIME_THREAD_ACTIVE: return NS_PER_SEC;
        default: return 0;
    }
}

// Human-readable time

const int _days_in_month[12] = {
    31,28, 31,30,31, 30,31,31, 30,31,30, 31
};

const int _month_days_since_year_start[12] = {
    0,
    31,
    31+28,
    31+28+31,
    31+28+31+30,
    31+28+31+30+31,
    31+28+31+30+31+30,
    31+28+31+30+31+30+31,
    31+28+31+30+31+30+31+31,
    31+28+31+30+31+30+31+31+30,
    31+28+31+30+31+30+31+31+30+31,
    31+28+31+30+31+30+31+31+30+31+30,
};

static bool _is_leap_year(int year) {
    if(year % 400 == 0) {
        return true;
    }
    if(year % 100 == 0) {
        return false;
    }
    if(year % 4 == 0) {
        return true;
    }
    return false;
}

static int _leap_years_before(int year) {
    year--;
    return (year / 4) - (year / 100) + (year / 400);
}

static int _leap_years_between(int start, int end) {
    return _leap_years_before(end) - _leap_years_before(start + 1);
}

static int _whole_days_since_year_start(int year, int mon, int day) {
    int days = _month_days_since_year_start[mon] + day;
    if(mon > 1 || (mon == 1 && day > 27)) {
        if(_is_leap_year(year)) {
            days += 1;
        }
    }
    return days;
}

static void _tm_adjust(tm_t *time) {
    long long year = 1900 + time->tm_year;
    long long mon = time->tm_mon;
    long long day = time->tm_mday - 1;
    long long hour = time->tm_hour;
    long long min = time->tm_min;
    long long sec = time->tm_sec;
    // Adjust
    if(sec < 0) {
        min -= (-sec+59)/60;
        sec = (sec%60 + 60) % 60;
    }
    else if(sec >= 60) {
        min += sec / 60;
        sec %= 60;
    }
    if(min < 0) {
        hour -= (-min+59)/60;
        min = (min%60 + 60) % 60;
    }
    else if(min >= 60) {
        hour += min / 60;
        min %= 60;
    }
    if(hour < 0) {
        day -= (-hour+23)/24;
        hour = (hour%24 + 24) % 24;
    }
    else if(hour >= 24) {
        day += hour / 24;
        hour %= 24;
    }
    if(mon < 0) {
        year -= (-mon+11)/12;
        mon = (mon%12+12)%12;
    }
    else if(mon >= 12) {
        year += mon / 12;
        mon %= 12;
    }
    if(day < 0) {
        for(;;) {
            int mon_prev = mon - 1;
            int year_prev = year;
            if(mon_prev == -1) {
                mon_prev = 11;
                year_prev -= 1;
            }
            int days_in_prev_month = _days_in_month[mon_prev];
            if(mon_prev == 1 && _is_leap_year(year_prev)) {
                days_in_prev_month += 1;
            }
            if(-day > days_in_prev_month) {
                day += days_in_prev_month;
                mon -= 1;
                if(mon == -1) {
                    mon = 11;
                    year -= 1;
                }
            }
            else {
                break; // Can't subtract any whole months, exit the loop
            }
        }
        if(day < 0) {
            mon -= 1;
            day = _days_in_month[mon] + day;
            if(mon == -1) {
                mon = 11;
                year -= 1;
            }
        }
    }
    else {
        for(;;) {
            int days_in_month = _days_in_month[mon];
            if(mon == 1 && _is_leap_year(year)) {
                days_in_month += 1;
            }
            if(day < days_in_month) {
                break;
            }
            day -= days_in_month;
            mon += 1;
        }
    }
    // Refill the struct with normalized time
    time->tm_year = year - 1900;
    time->tm_mon = mon;
    time->tm_mday = day + 1;
    time->tm_hour = hour;
    time->tm_min = min;
    time->tm_sec = sec;
}

time_t mktime(tm_t *time) {
    // Get the human-readable time from the structure
    _tm_adjust(time);
    long long year = 1900 + time->tm_year;
    long long mon = time->tm_mon;
    long long day = time->tm_mday - 1;
    long long hour = time->tm_hour;
    long long min = time->tm_min;
    long long sec = time->tm_sec;
    if(year < 1970) {
        return (time_t)(-1);
    }
    // Calculate unix timestamp
    time_t timestamp;
    timestamp = (year - 1970) * 365 + _leap_years_between(1970, year);
    time_t days_since_year = _whole_days_since_year_start(year, mon, day);
    timestamp = timestamp + days_since_year;
    time_t days_since_epoch = timestamp;
    timestamp = timestamp * 24 + hour;
    timestamp = timestamp * 60 + min;
    timestamp = timestamp * 60 + sec;
    timestamp = timestamp * NS_PER_SEC;
    // Refill the week day and year day
    time->tm_wday = (days_since_epoch % 7 + 4) % 7; // 1970-01-01 is thursday (wday=4)
    time->tm_yday = days_since_year;
    time->tm_isdst = -1;
    return timestamp;
}

// Breaking-down of the time

tm_t *gmtime(const time_t *timer) {
    tm_t *time = malloc(sizeof(tm_t));
    time_t timestamp = *timer;
    timestamp /= NS_PER_SEC;
    time->tm_sec = timestamp % 60;
    timestamp /= 60;
    time->tm_min = timestamp % 60;
    timestamp /= 60;
    time->tm_hour = timestamp % 24;
    timestamp /= 24;
    int year = 1970;
    int days = timestamp;
    // Start subtracting 400,100,4-year-stretches
    const int DAYS_IN_400_YEARS = 365 * 400 + 97;
    const int DAYS_IN_100_YEARS = 365 * 100 + 24;
    const int DAYS_IN_4_YEARS   = 365 * 4 + 1;
    year += (days / DAYS_IN_400_YEARS) * 400;
    days %= DAYS_IN_400_YEARS;
    year += (days / DAYS_IN_100_YEARS) * 100;
    days %= DAYS_IN_100_YEARS;
    year += (days / DAYS_IN_4_YEARS) * 4;
    days %= DAYS_IN_4_YEARS;
    // Save
    int days_since_epoch = days;
    // Count remaining up-to 3 years in a loop
    for(;;) {
        int this_year_days = 365 + _is_leap_year(year);
        if(days < this_year_days) {
            break;
        }
        year += 1;
        days -= this_year_days;
    }
    // Save
    int days_since_year = days;
    // Find the current month
    int month = 0;
    for(;;) {
        int days_in_month = _days_in_month[month] + _is_leap_year(year);
        if(days < days_in_month) {
            break;
        }
        days -= days_in_month;
        month += 1;
    }
    time->tm_year = year - 1900;
    time->tm_mon = month;
    time->tm_mday = days + 1;
    time->tm_wday = (days_since_epoch % 7 + 4) % 7; // 1970-01-01 is thursday (wday=4)
    time->tm_yday = days_since_year;
    time->tm_isdst = -1;
    return time;
}

static bool _offset_utc_time_to_local(tm_t *time) {
    TIME_ZONE_INFORMATION tz;
    if(GetTimeZoneInformation(&tz) == TIME_ZONE_ID_INVALID) {
        return false;
    }
    int bias = tz.Bias;
    time->tm_min -= bias;
    _tm_adjust(time);
    return true;
}

tm_t *localtime(const time_t *timer) {
    tm_t *utc_time = gmtime(timer);
    if(!_offset_utc_time_to_local(utc_time)) {
        return NULL;
    }
    return utc_time;
}

// String formatting

char *asctime(const tm_t *timeptr) {
    return NULL;
}
char *ctime(const time_t *timer) {
    return NULL;
}

size_t strftime(char *restrict s, size_t size, const char *restrict fmt, const tm_t *restrict time) {
    return 0;
}
