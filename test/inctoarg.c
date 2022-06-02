#include<stdio.h>
#include<stdlib.h>

int main()
{
    char *include = getenv("INCLUDE");
    printf(" ");
    while(*include != 0) {
        printf("-I \"");
        while(*include != ';' && *include != 0) {
            printf("%c", *include);
            *include ++;
        }
        if(*include == ';') {
            ++include;
        }
        printf("\" ");
    }
}
