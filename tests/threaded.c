
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

int thrd_func(void *arg) {
    char string[] = "child thread: ok!\n";
    fwrite(string, 1, sizeof string-1, stdout);
    // exit(1);
    for(;;);
    return 0;
}

int main() {
    {char string[] = "main thred: before\n";
    fwrite(string, 1, sizeof string-1, stdout);}
    thrd_t thrd;
    thrd_create(&thrd, thrd_func, NULL);
    {char string[] = "main thread: after!\n";
    fwrite(string, 1, sizeof string-1, stdout);}
    return 0;
}
