// https://en.cppreference.com/w/c/algorithm/bsearch
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct data {
    int nr;
    char const *value;
} dat[] = {
    {1, "Foo"}, {2, "Bar"}, {3, "Hello"}, {4, "World"}
};

int data_cmp(void const *lhs, void const *rhs)
{
    struct data const *const l = lhs;
    struct data const *const r = rhs;

    if (l->nr < r->nr) return -1;
    else if (l->nr > r->nr) return 1;
    else return 0;
}

int main(void)
{
    struct data key = { .nr = 3 };
    struct data const *res = bsearch(&key, dat, sizeof dat / sizeof dat[0], sizeof dat[0], data_cmp);
    if (res) {
        printf("No %d: %s\n", res->nr, res->value);
    } else {
        printf("No %d not found\n", key.nr);
    }

    while (clock() <= 3*CLOCKS_PER_SEC) {
        printf("%llu seconds\n", clock() / CLOCKS_PER_SEC);
    }

    struct timespec start;
    timespec_get(&start, TIME_UTC);

    struct timespec ts;
    do {
        timespec_get(&ts, TIME_UTC);
        printf("%zu:%ld\n", ts.tv_sec, ts.tv_nsec);
    } while (ts.tv_sec < start.tv_sec+5);
}
