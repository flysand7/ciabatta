#include <stdio.h>
#include <stdint.h>
int main() {
    uint8_t *sp = _alloca(512);
    for(int i = 0; i != 550; ++i) {
        sp[i] = 0;
    }
    printf("%p\n", sp);
    return 0;
}
