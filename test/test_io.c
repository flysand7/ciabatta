
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
int main(void)
{
    // prepare a file holding 4 values of type char
    enum {SIZE = 4};
    FILE* fp = fopen("test.bin", "wb");
    assert(fp);
    fputc('a', fp);
    fputc('b', fp);
    fputc('d', fp);
    fputc('c', fp);
    fclose(fp);
 
    // demo using fsetpos to return to the beginning of a file
    fp = fopen("test.bin", "rb");
    fpos_t pos;
    int c = fgetc(fp);
    printf("First value in the file: %c\n", c);
    fgetpos(fp, &pos);
    c = fgetc(fp);
    printf("Second value in the file: %c\n", c);
    fsetpos(fp,&pos);
    c = fgetc(fp);
    printf("Second value in the file again: %c\n", c);
    fclose(fp);
    return 0;
}
