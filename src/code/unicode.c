
#include <unicode.h>

#include "unicode/data.h"

uchar_props *uni_props(uchar_t cp) {
    if(!uni_valid(cp))              return NULL;
    if(unicode_data[cp].code != cp) return NULL;
    return &unicode_data[cp];
}

int uni_cat_gen(uchar_t cp) {
    uchar_props *props = uni_props(cp);
    if(props != NULL)
        return unicode_data[cp].cat_gen;
    else
        return UCHAR_BAD;
}

uchar_t uni_tolower(uchar_t cp) {
    return unicode_data[cp].lower;
}

uchar_t uni_toupper(uchar_t cp) {
    return unicode_data[cp].upper;
}

uchar_t uni_totitle(uchar_t cp) {
    return unicode_data[cp].title;
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
    else if(uni_is_lsur(cp)) return UNI_EULSUR;
    else return 1;
}

int utf8_chlen(char const *str) {
    uint8_t byte0 = (uint8_t)*str;
    if(byte0 < 0x80)      return 1;
    else if(byte0 < 0xc0) return UNI_EIBYTE;
    else if(byte0 < 0xe0) return 2;
    else if(byte0 < 0xf0) return 3;
    else if(byte0 < 0xf8) return 4;
    return UNI_EIBYTE;
}

int utf16_dec(char16_t const *restrict str, uchar_t *restrict chp) {
    int chlen = 0;
    uchar_t ch;
    if(uni_is_hsur(str[0])) {
        char16_t hsur = str[0];
        char16_t lsur = str[1];
        ch = uni_surtoc(hsur, lsur);
        chlen = 2;
        if(ch > 0x10ffff) {
            chlen = UNI_EBADCP;
            ch = 0xfffd;
        }
    }
    else(!uni_is_lsur(str[0])) {
        ch = str[0];
    }
    else {
        chlen = UNI_EULSUR;
        ch = 0xfffd;
    }
    if(chp != NULL) *chp = ch;
    return chlen;
}

int utf16_dec_s(
    char16_t const *restrict str,
    int len,
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

int utf8_dec(char const *restrict str, uchar_t *restrict chp) {
    uint8_t const *ustr = (uint8_t const *)str;

    int chlen;
    uchar_t ch;
    if(ustr[0] < 0x80)      ch = ustr[0],        chlen = 1;
    else if(ustr[0] < 0xc0) ch = 0xfffd,         chlen = UNI_EIBYTE;
    else if(ustr[0] < 0xe0) ch = ustr[0] & 0x1f, chlen = 2;
    else if(ustr[0] < 0xf0) ch = ustr[0] & 0x0f, chlen = 3;
    else if(ustr[0] < 0xf8) ch = ustr[0] & 0x07, chlen = 4;
    else                    ch = 0xfffd,         chlen = UNI_EIBYTE;

    if(chlen > 0) for(int i = 1; i < chlen; ++i) {
        uint8_t trail = ustr[i];
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

int utf8_dec_s(
    char const *restrict str,
    int len,
    uchar_t *restrict chp
) {
    if(len == 0) return 0;
    uint8_t const *restrict ustr = (uint8_t const *restrict)str;
    int chlen;
    uchar_t ch;
    if(ustr[0] < 0x80)      ch = ustr[0],        chlen = 1;
    else if(ustr[0] < 0xc0) ch = 0xfffd,         chlen = UNI_EIBYTE;
    else if(ustr[0] < 0xe0) ch = ustr[0] & 0x1f, chlen = 2;
    else if(ustr[0] < 0xf0) ch = ustr[0] & 0x0f, chlen = 3;
    else if(ustr[0] < 0xf8) ch = ustr[0] & 0x07, chlen = 4;
    else                    ch = 0xfffd,         chlen = UNI_EIBYTE;
    if(chlen > len) {
        return UNI_ESTRLN;
    }
    if(chlen > 0) for(int i = 1; i < chlen; ++i) {
        uint8_t trail = ustr[i];
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

int utf16_enc(char16_t *str, uchar_t cp) {
    if(!is_valid(cp)) {
        return UNI_EBADCP;
    }
    if(cp < 0x10000) {
        str[0] = cp;
        return 1;
    }
    else {
        cp -= 0x10000;
        str[0] = 0xD800 + (cp >> 10);
        str[1] = 0xDC00 + (cp & 0x3ff);
        return 2;
    }
}

int utf8_enc(char *str, uchar_t ch) {
    if(!is_valid(cp)) {
        return UNI_EBADCP;
    }
    if(cp < 0x80) {
        str[0] = ch;
        return 1;
    }
    else if(cp < 0x800) {
        str[0] = 0xc0 | (ch >> 6);
        str[1] = 0x80 | ((ch >> 0) & 0x3f);
        return 2;
    }
    else if(cp < 0x10000) {
        str[0] = 0xe0 | (ch >> 18);
        str[1] = 0x80 | ((ch >> 6) & 0x3f);
        str[2] = 0x80 | ((ch >> 0) & 0x3f);
        return 3;
    }
    else {
        str[0] = 0xe0 | (ch >> 24);
        str[1] = 0x80 | ((ch >> 18) & 0x3f);
        str[2] = 0x80 | ((ch >> 6)  & 0x3f);
        str[3] = 0x80 | ((ch >> 0)  & 0x3f);
        return 4;
    }
}

int utf16_enc_s(char16_t *str, size_t len, uchar_t ch) {
    if(!is_valid(cp)) {
        return UNI_EBADCP;
    }
    if(len == 0) return 0;
    if(cp < 0x10000) {
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

int utf8_enc_s(char *str, size_t len, uchar_t ch) {
    if(!is_valid(cp)) {
        return UNI_EBADCP;
    }
    if(len == 0) return 0;
    if(cp < 0x80) {
        str[0] = ch;
        return 1;
    }
    else if(cp < 0x800) {
        if(len < 2) return UNI_ESTRLN;
        str[0] = 0xc0 | (ch >> 6);
        str[1] = 0x80 | ((ch >> 0) & 0x3f);
        return 2;
    }
    else if(cp < 0x10000) {
        if(len < 3) return UNI_ESTRLN;
        str[0] = 0xe0 | (ch >> 18);
        str[1] = 0x80 | ((ch >> 6) & 0x3f);
        str[2] = 0x80 | ((ch >> 0) & 0x3f);
        return 3;
    }
    else {
        if(len < 4) return UNI_ESTRLN;
        str[0] = 0xe0 | (ch >> 24);
        str[1] = 0x80 | ((ch >> 18) & 0x3f);
        str[2] = 0x80 | ((ch >> 6)  & 0x3f);
        str[3] = 0x80 | ((ch >> 0)  & 0x3f);
        return 4;
    }
}
