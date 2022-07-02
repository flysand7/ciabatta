
#include <unicode.h>
#include <stdio.h>

int main() {
    char *mbstr = u8"улыбок тебе дед макар";
    {
        char *str = mbstr;
        uchar_t ch;
        int len;
        while((len = utf8_dec(str, &ch)) > 0 && ch != 0) {
            printf("char: %d\n", ch);
            str += len;
        }
        if(len <= 0) {
            printf("This string is not utf8\n");
        }
    }
}
