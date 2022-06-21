
#include <wctype.h>
#include <string.h>

#include "unicode/data.h"

static inline int char_cat(wint_t wc) {
    return uni_codepoints[wc].cat;
}

int iswctype(wint_t wc, wctype_t desc) {
    return desc(wc);
}

wctype_t wctype(const char *property) {
    if(!strcmp(property, "alnum"))  return iswalnum;
    if(!strcmp(property, "alpha"))  return iswalpha;
    if(!strcmp(property, "blank"))  return iswblank;
    if(!strcmp(property, "cntrl"))  return iswcntrl;
    if(!strcmp(property, "digit"))  return iswdigit;
    if(!strcmp(property, "graph"))  return iswgraph;
    if(!strcmp(property, "lower"))  return iswlower;
    if(!strcmp(property, "print"))  return iswprint;
    if(!strcmp(property, "punct"))  return iswpunct;
    if(!strcmp(property, "space"))  return iswspace;
    if(!strcmp(property, "upper"))  return iswupper;
    if(!strcmp(property, "xdigit")) return iswxdigit;
    return NULL;
}

wint_t towctrans(wint_t wc, wctrans_t desc) {
    return desc(wc);
}

wctrans_t wctrans(const char *property) {
    if(!strcmp(property, "tolower"))  return towlower;
    if(!strcmp(property, "toupper"))  return towupper;
    return NULL;
}

int iswalnum(wint_t wc) {
    return iswalpha(wc) || iswdigit(wc);
}

int iswalpha(wint_t wc) {
    return iswupper(wc) || iswlower(wc);
}

int iswblank(wint_t wc) {
    return wc == ' ' || wc == '\t';
}

int iswcntrl(wint_t wc) {
    return char_cat(wc) == Cc;
}

int iswdigit(wint_t wc) {
    return '0' <= wc && wc <= '9';
}

int iswgraph(wint_t wc) {
    return iswprint(wc) && !iswspace(wc);
}

int iswlower(wint_t wc) {
    return char_cat(wc) == Ll;
}

int iswprint(wint_t wc) {
    switch(char_cat(wc)) {
        case Cc:
        case Cf:
        case Co:
        case Cs:
            return 0;
    }
    return 1;
}

int iswpunct(wint_t wc) {
    switch(char_cat(wc)) {
        case Pc:
        case Pd:
        case Pe:
        case Pf:
        case Pi:
        case Po:
        case Ps:
        case Sk:
        case Sc:
        case Sm:
        case So:
            return 1;
    }
    return 0;
}

int iswspace(wint_t wc) {
    switch(wc) {
        case ' ':
        case '\t':
        case '\v':
        case '\r':
        case '\n':
        case '\f':
            return 1;
    }
    return 0;
}

int iswupper(wint_t wc) {
    return char_cat(wc) == Lu;
}

int iswxdigit(wint_t wc) {
    return iswdigit(wc) || ('a'<=wc && wc<='f') || ('A'<= wc && wc<='F');
}

wint_t towlower(wint_t wc) {
    return uni_codepoints[wc].lower;
}

wint_t towupper(wint_t wc) {
    return uni_codepoints[wc].upper;
}
