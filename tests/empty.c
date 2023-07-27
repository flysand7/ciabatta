
#include <cia-def.h>
#include <stdio.h>

int main(int argc, char **argv, char **envp) {
    char string[] = "Hello, world!\n";
    fwrite(string, 1, sizeof string-1, stdout);
    return 0;
}
