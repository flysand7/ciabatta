
#include <unicope.h>

int utf8_chlen(char8_t const *str) {
    char8_t byte0 = *str;
    if(byte0 < 0x80)      return 1;
    else if(byte0 < 0xc0) return UNI_EIBYTE;
    else if(byte0 < 0xe0) return 2;
    else if(byte0 < 0xf0) return 3;
    else if(byte0 < 0xf8) return 4;
    return UNI_EIBYTE;
}

int utf8_chdec(
    char8_t const *restrict str,
    size_t len,
    char32_t *restrict chp
) {
    if(len == 0) return 0;
    int chlen;
    char32_t ch;
    if(str[0] < 0x80)      ch = str[0],        chlen = 1;
    else if(str[0] < 0xc0) ch = 0xfffd,        chlen = UNI_EIBYTE;
    else if(str[0] < 0xe0) ch = str[0] & 0x1f, chlen = 2;
    else if(str[0] < 0xf0) ch = str[0] & 0x0f, chlen = 3;
    else if(str[0] < 0xf8) ch = str[0] & 0x07, chlen = 4;
    else                   ch = 0xfffd,        chlen = UNI_EIBYTE;
    if(chlen > len) {
        return UNI_ESTRLN;
    }
    if(chlen > 0) for(int i = 1; i < chlen; ++i) {
        uint8_t trail = str[i];
        if((trail & 0xc0) != 0x80) {
            chlen = UNI_ETBYTE;
            ch = 0xfffd;
            break;
        }
        ch <<= 6;
        ch |= (trail & 0x3f); 
    }
    if(!uni_valid(ch)) {
        chlen = UNI_EBADCP;
        ch = 0xfffd;
    }
    if(chp != NULL) *chp = ch;
    return chlen;
}

int utf8_chenc(char8_t *str, size_t len, char32_t cp) {
    if(!uni_valid(cp)) {
        return UNI_EBADCP;
    }
    if(len == 0) return 0;
    if(cp < 0x80) {
        str[0] = cp;
        return 1;
    }
    else if(cp < 0x800) {
        if(len < 2) return UNI_ESTRLN;
        str[0] = 0xc0 | (cp >> 6);
        str[1] = 0x80 | ((cp >> 0) & 0x3f);
        return 2;
    }
    else if(cp < 0x10000) {
        if(len < 3) return UNI_ESTRLN;
        str[0] = 0xe0 | (cp >> 18);
        str[1] = 0x80 | ((cp >> 6) & 0x3f);
        str[2] = 0x80 | ((cp >> 0) & 0x3f);
        return 3;
    }
    else {
        if(len < 4) return UNI_ESTRLN;
        str[0] = 0xe0 | (cp >> 24);
        str[1] = 0x80 | ((cp >> 18) & 0x3f);
        str[2] = 0x80 | ((cp >> 6)  & 0x3f);
        str[3] = 0x80 | ((cp >> 0)  & 0x3f);
        return 4;
    }
}
