
#include <unicode.h>

#include "unicode/data.h"

int uni_classify(uchar_t cp) {
    return uni_codepoints[cp].cat;
}

uchar_t uni_to_lower(uchar_t cp) {
    return uni_codepoints[cp].lower;
}

uchar_t uni_to_upper(uchar_t cp) {
    return uni_codepoints[cp].upper;
}

int uni_valid(uchar_t ch) {
    return (0x0000 <= ch && ch <= 0xd7ff) || (0xe000 <= ch && ch <= 0x10ffff);
}

int uni_is_hsur(char16_t ch) {
    return 0xd800 <= ch && ch <= 0xdbff;
}

int uni_is_lsur(char16_t ch) {
    return 0xdc00 <= ch && ch <= 0xdfff;
}

uchar_t uni_surtoc(char16_t hsur, char16_t lsur) {
    uchar_t u = ((0x3ff & hsur) << 10) | (lsur & 0x3ff);
    return u + 0x10000;
}

int utf16_chlen(char16_t const *str) {
    char16_t cp = *str;
    if(uni_is_hsur(cp))      return 2;
    else if(uni_is_lsur(cp)) return 0;
    else if(uni_valid(cp))   return 1;
    return 0;
}

int utf8_chlen(char const *str) {
    uint8_t byte0 = (uint8_t)*str;
    if(byte0 < 0x80)      return 1;
    else if(byte0 < 0xc0) return 0; // error
    else if(byte0 < 0xe0) return 2;
    else if(byte0 < 0xf0) return 3;
    else if(byte0 < 0xf8) return 4;
    return 0;
}

int utf16_dec(char16_t const *restrict str, uchar_t *restrict chp) {
    int chlen = 0;
    uchar_t ch;
    if(uni_is_hsur(str[0])) {
        char16_t hsur = str[0];
        char16_t lsur = str[1];
        ch = uni_surtoc(hsur, lsur);
        chlen = 2;
    }
    else {
        ch = str[0];
    }
    if(!uni_valid(ch)) {
        chlen = 0;
        ch = 0xfffd;
    }
    if(chp != NULL) *chp = ch;
    return chlen;
}

int utf16_dec_s(
    char16_t const *restrict str,
    size_t len,
    uchar_t *restrict chp
) {
    if(len == 0) return 0;
    int chlen;
    uchar_t ch;
    if(uni_is_hsur(str[0])) {
        if(len < 2) return 0;
        char16_t hsur = str[0];
        char16_t lsur = str[1];
        ch = uni_surtoc(hsur, lsur);
        chlen = 2;
    }
    else {
        ch = str[0];
        chlen = 1;
    }
    if(!uni_valid(ch)) {
        ch = 0xfffd;
        chlen = 0;
    }
    if(chp != NULL) *chp = ch;
    return chlen;
}

int utf8_dec(char const *restrict str, uchar_t *restrict chp) {
    uint8_t const *ustr = (uint8_t const *)str;
    int chlen;
    uchar_t ch;
    if(ustr[0] < 0x80)      chlen = 1, ch = ustr[0];
    else if(ustr[0] < 0xc0) chlen = 0, ch = 0xfffd;
    else if(ustr[0] < 0xe0) chlen = 2, ch = ustr[0] & 0x1f;
    else if(ustr[0] < 0xf0) chlen = 3, ch = ustr[0] & 0x0f;
    else if(ustr[0] < 0xf8) chlen = 4, ch = ustr[0] & 0x07;
    else chlen = 0;
    for(int i = 1; i < chlen; ++i) {
        uint8_t trail = ustr[i];
        if((trail & 0xc0) != 0x80) {
            chlen = 0;
            ch = 0xfffd;
            break;
        }
        ch <<= 6;
        ch |= (trail & 0x3f); 
    }
    if(!uni_valid(ch)) {
        chlen = 0;
        ch = 0xfffd;
    }
    if(chp != NULL) *chp = ch;
    return chlen;
}

int utf8_dec_s(
    char const *restrict str,
    size_t len,
    uchar_t *restrict chp
) {
    if(len == 0) return 0;
    uint8_t const *restrict ustr = (uint8_t const *restrict)str;
    int chlen;
    uchar_t ch;
    if(ustr[0] < 0x80)      chlen = 1, ch = ustr[0];
    else if(ustr[0] < 0xc0) chlen = 0, ch = 0xfffd;
    else if(ustr[0] < 0xe0) chlen = 2, ch = ustr[0] & 0x1f;
    else if(ustr[0] < 0xf0) chlen = 3, ch = ustr[0] & 0x0f;
    else if(ustr[0] < 0xf8) chlen = 4, ch = ustr[0] & 0x07;
    else chlen = 0;
    if(len < chlen) {
        return 0;
    }
    else chlen = 0;
    for(int i = 1; i < chlen; ++i) {
        uint8_t trail = ustr[i];
        if((trail & 0xc0) != 0x80) {
            chlen = 0;
            ch = 0xfffd;
            break;
        }
        ch <<= 6;
        ch |= (trail & 0x3f); 
    }
    if(!uni_valid(ch)) {
        chlen = 0;
        ch = 0xfffd;
    }
    if(chp != NULL) *chp = ch;
    return chlen;
}

