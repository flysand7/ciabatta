
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

