
#include <assert.h>
#include <stddef.h>
#include <errno.h>
void do_more_stuff(char *ptr) {
    assert(ptr != NULL);
}

void do_stuff() {
    do_more_stuff(NULL);
}

int main() {
    errno = 0;
    assert(2+2 == 4);
    do_stuff();
}
