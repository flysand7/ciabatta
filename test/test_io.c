
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void) {
    FILE *tmp = tmpfile();
    fputc('a', tmp);
    fclose(tmp);
    return 0;
}
