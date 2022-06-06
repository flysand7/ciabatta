#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <inttypes.h>

int main(int argc, char** argv) {
    uint64_t mynumber = 4;
    printf("Hello, guys %"PRIu64"\n", mynumber);
    return 0;
}