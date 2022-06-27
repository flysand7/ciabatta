
#include <time.h>
#include <stdio.h>

int main() {
    time_t t1 = time(NULL);
    time_t t2 = time(NULL);
    double s = difftime(t2, t1);
    return 0;
}
