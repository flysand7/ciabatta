
#include <unicope.h>

#include "data/unicode.h"
#define countof(tab) (sizeof(tab)/sizeof(tab)[0])

int uni_valid(char32_t ch) {
    return (0x0000 <= ch && ch <= 0xd7ff) || (0xe000 <= ch && ch <= 0x10ffff);
}

int uni_is_hsur(char16_t ch) {
    return 0xd800 <= ch && ch <= 0xdbff;
}

int uni_is_lsur(char16_t ch) {
    return 0xdc00 <= ch && ch <= 0xdfff;
}

char32_t uni_surtoc(char16_t hsur, char16_t lsur) {
    char32_t u = ((0x3ff & hsur) << 10) | (lsur & 0x3ff);
    return u + 0x10000;
}


int uni_classify(char32_t cp) {
    if(!uni_valid(cp)) {
        return -1;
    }
    int range = cp >> 7;
    if(range < countof(cat_block_indices)) {
        return -1;
    }
    int offset = cp & 0x7f;
    int block = cat_block_indices[range];
    int cat = char_cat_props[block][offset].cat_gen & UCHAR_CAT_GEN_MASK;
    return cat;
}

static const char32_t fdfa_map[] = {
    0x0635, 0x0644, 0x0649, 0x0020, 0x0627, 0x0644,
    0x0644, 0x0647, 0x0020, 0x0639, 0x0644, 0x064a,
    0x0647, 0x0020, 0x0648, 0x0633, 0x0644, 0x0645,
};

char32_t const *uni_dec_map(char32_t cp, int *num) {
    if(cp == 0xfdfa) {
        *num = 18;
        return fdfa_map;
    }
    if(cp < 0x10000) {
        if(unicode_data16[cp].code == cp) {
            *num = unicode_data16[cp].dec_map_n;
            return unicode_data16[cp].dec_map;
        }
    }
    else if((cp - 0x10000) < countof(unicode_data32)) {
        if(unicode_data32[cp-0x10000].code == cp) {
            *num = unicode_data32[cp-0x10000].dec_map_n;
            return unicode_data32[cp-0x10000].dec_map;
        }
    }
    return NULL;
}


char32_t uni_tolower(char32_t cp) {
    if(!uni_valid(cp)) {
        return cp;
    }
    if(cp < 0x10000) {
        if(unicode_data16[cp].code == cp) {
            return unicode_data16[cp].lower;
        }
    }
    else if((cp - 0x10000) < countof(unicode_data32)) {
        if(unicode_data32[cp-0x10000].code == cp) {
            return unicode_data32[cp-0x10000].lower;
        }
    }
    return cp;
}

char32_t uni_toupper(char32_t cp) {
    if(!uni_valid(cp)) {
        return cp;
    }
    if(cp < 0x10000) {
        if(unicode_data16[cp].code == cp) {
            return unicode_data16[cp].upper;
        }
    }
    else if((cp - 0x10000) < countof(unicode_data32)) {
        if(unicode_data32[cp-0x10000].code == cp) {
            return unicode_data32[cp-0x10000].upper;
        }
    }
    return cp;
}

char32_t uni_totitle(char32_t cp) {
    if(uni_valid(cp)) {
        return cp;
    }
    if(cp < 0x10000) {
        if(unicode_data16[cp].code == cp) {
            return unicode_data16[cp].title;
        }
    }
    else if((cp - 0x10000) < countof(unicode_data32)) {
        if(unicode_data32[cp-0x10000].code == cp) {
            return unicode_data32[cp-0x10000].title;
        }
    }
    return cp;
}
