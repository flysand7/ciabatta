
#include <stdio.h>

int main(void)
{
    char const *filename = "todo";
    FILE *f = fopen(filename, "rb");
    if(f == NULL) {
        printf("File %s doesn't exist\n", filename);
        return 1;
    }
    int c;
    while((c=fgetc(f)) != EOF) {
        putchar(c);
    }
    return 0;
}
