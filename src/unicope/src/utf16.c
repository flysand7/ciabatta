
#include <unicope.h>

int utf16_chlen(char16_t const *str) {
    char16_t cp = *str;
    if(uni_is_hsur(cp))      return 2;
    else if(uni_is_lsur(cp)) return UNI_EULSUR;
    else return 1;
}

int utf16_chdec(
    char16_t const *restrict str,
    size_t len,
    char32_t *restrict chp
) {
    if(len == 0) return 0;
    int chlen;
    char32_t ch;
    if(uni_is_hsur(str[0])) {
        if(len < 2) return UNI_ESTRLN;
        char16_t hsur = str[0];
        char16_t lsur = str[1];
        ch = uni_surtoc(hsur, lsur);
        chlen = 2;
        if(ch > 0x10ffff) {
            chlen = UNI_EBADCP;
            ch = 0xfffd;
        }
    }
    else if(!uni_is_lsur(str[0])) {
        ch = str[0];
        chlen = 1;
    }
    else {
        chlen = UNI_EULSUR;
        ch = 0xfffd;
    }
    if(chp != NULL) *chp = ch;
    return chlen;
}

int utf16_chenc(char16_t *str, size_t len, char32_t cp) {
    if(!uni_valid(cp)) {
        return UNI_EBADCP;
    }
    if(cp < 0x10000) {
        if(len < 1) return UNI_ESTRLN;
        str[0] = cp;
        return 1;
    }
    else {
        if(len < 2) return UNI_ESTRLN;
        cp -= 0x10000;
        str[0] = 0xD800 + (cp >> 10);
        str[1] = 0xDC00 + (cp & 0x3ff);
        return 2;
    }
}
