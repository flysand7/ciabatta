
#include <ctype.h>
#include <wctype.h>

#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int main() {
    float start = (float)clock()/CLOCKS_PER_SEC;
    for(int i = 0; i != 1000000; ++i) {
        int c = rand() % 128;
        assert(ispunct(c) == iswpunct((wint_t)c));
        assert(isalpha(c) == iswalpha((wint_t)c));
        assert(isspace(c) == iswspace((wint_t)c));
        assert(isgraph(c) == iswgraph((wint_t)c));
        assert(iscntrl(c) == iswcntrl((wint_t)c));
    }
    float end = (float)clock()/CLOCKS_PER_SEC;
    float elaps = end-start;
    printf("Elapsed: %f\n", elaps);
    return 0;
}
