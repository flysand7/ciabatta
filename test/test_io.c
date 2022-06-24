
#include <stdio.h>

int main() {
    for(int i = 0; i != 512; ++i)
        fputc('Z', stdout);
    return 0;
}
