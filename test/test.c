#include <assert.h>
#include <ctype.h>

int main(int argc, char** argv) {
    /*for (int i = 0; i < argv; i++) {
        printf("[%d] = %s\n", argv[i]);
    }*/

    for (char c = 'a'; c != 'z'; ++c) {
        assert(isupper(toupper(c)));
    }
    return 0;
}
