
#include <cia/def.h>
#include <stdio.h>

_Thread_local u64 my_thread_local;

int main(void) {
    int arr[16] = {0};
    my_thread_local = 2;
    arr[3] = 5;
    char string[] = "Hello, world!\n";
    fwrite(string, 1, sizeof string-1, stdout);
    return 0;
}
