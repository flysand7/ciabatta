#include <stdio.h>
#include <time.h>
static const char *const wday[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday",
    "Thursday", "Friday", "Saturday", "-unknown-"
};

int main() {
    // July 4, 2001: Wednesday
    struct tm time_str;
    time_str.tm_year = 2001 - 1900;
    time_str.tm_mon = 7 - 1;
    time_str.tm_mday = 4;
    time_str.tm_hour = 0;
    time_str.tm_min = 0;
    time_str.tm_sec = 1;
    time_str.tm_isdst = -1;
    time_t timestamp;
    if((timestamp = mktime(&time_str)) == (time_t)(-1)) {
        time_str.tm_wday = 7;
    }
    char const *day_of_week = wday[time_str.tm_wday];
    struct tm *gmt = gmtime(&timestamp);
    struct tm *local = localtime(&timestamp);
    return 0;
}