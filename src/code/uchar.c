
#include <uchar.h>
#include <errno.h>

#include <unicode.h>

size_t mbrtoc16(
    char16_t   *restrict pc16,
    char const *restrict s,
    size_t               n,
    mbstate_t  *restrict ps
) {
    if(s == NULL) {
        *ps = (mbstate_t) {0};
        return 0;
    }
    // First check leftovers, using 0xd800 as marker because it doesn't
    // encode a valid character.
    if(ps->leftover != 0xd800) {
        if(pc16 != NULL) *pc16 = ps->leftover;
        ps->leftover = 0xd800;
        return (size_t)(-3);
    }
    else {
        uchar_t ch;
        char16_t str[3];
        int chlen = utf8_dec(s, &ch);
        if(chlen <= 0) goto encoding_error;
        int wrlen = utf16_enc(str, ch);
        char16_t curc;
        char16_t next;
        if(wrlen <= 0) goto encoding_error;
        else if(wrlen == 2) {
            curc = str[0];
            next = 0xd800;
        }
        else {
            curc = str[0];
            next = str[1];
        }
        ps->leftover = next;
        if(pc16 != NULL) *pc16 = curc;
        return (size_t)-2;
    }
encoding_error:
    errno = EILSEQ;
    return (size_t)(-1);
}

size_t c16rtomb(
    char *restrict      s,
    char16_t            c16,
    mbstate_t *restrict ps
) {
    if(s == NULL) {
        *ps = (mbstate_t) {0};
        return 0;
    }
    unsigned cp;
    // High surrogate (save)
    if(0xd800 <= c16 && c16 < 0xdc00) {
        ps->leftover = c16;
        return 0;
    }
    // Low surrogate (parse)
    else if(0xdc00 <= c16 && c16 < 0xe000) {
        if(ps->leftover == 0) goto encoding_error;
        cp = ((ps->leftover & 0x3ff) << 10) | (c16 & 0x3ff);
    }
    // Other char
    else {
        cp = c16;
    }
    size_t nbytes = 4;
    if(cp < 0x10000) nbytes = 3;
    if(cp < 0x800)   nbytes = 2;
    if(cp < 0x80)    nbytes = 1;
    switch(nbytes) {
        case 1: {
            s[0] = cp;
        } break;
        case 2: {
            s[0] = 0xc0 | (cp >> 6);
            s[1] = 0x80 | ((cp >> 0)  & 0x3f);
        } break;
        case 3: {
            s[0] = 0xe0 | (cp >> 12);
            s[1] = 0x80 | ((cp >> 6)  & 0x3f);
            s[2] = 0x80 | ((cp >> 0)  & 0x3f);
        } break;
        case 4: {
            s[0] = 0xf0 | (cp >> 18);
            s[1] = 0x80 | ((cp >> 12) & 0x3f);
            s[2] = 0x80 | ((cp >> 6)  & 0x3f);
            s[3] = 0x80 | ((cp >> 0)  & 0x3f);
        } break;
    }
    return nbytes;
encoding_error:
    errno = EILSEQ;
    return (size_t)(-1);
}

size_t mbrtoc32(
    char32_t   *restrict pc32,
    char const *restrict s,
    size_t               n,
    mbstate_t  *restrict ps
) {
    if(s == NULL) {
        *ps = (mbstate_t) {0};
        return 0;
    }
    size_t nbytes;
    
    // Decode the first byte of UTF-8 sequence
    unsigned byte0 = *s;
    if     (0x00 <= byte0 && byte0 < 0x80) nbytes = 1;
    else if(0xc0 <= byte0 && byte0 < 0xe0) nbytes = 2;
    else if(0xe0 <= byte0 && byte0 < 0xf0) nbytes = 3;
    else if(0xf0 <= byte0 && byte0 < 0xf8) nbytes = 4;
    else goto encoding_error;
    unsigned nbytesreq = nbytes;
    if(n < nbytesreq) {
        return (size_t)(-2);
    }
    char32_t cp = byte0;
    switch(nbytesreq) {
        case 2: cp &= 0x1f; break;
        case 3: cp &= 0x0f; break;
        case 4: cp &= 0x07; break;
    }
    while(--nbytesreq)
        cp |= (cp << 6) | ((*++s) & 0x3f);
    if(0xdc00 <= cp && cp <= 0xe000)
        goto encoding_error;
    // Overloing seqs
    if(cp < 0x80    && nbytes > 1) goto encoding_error;
    if(cp < 0x800   && nbytes > 2) goto encoding_error;
    if(cp < 0x10000 && nbytes > 3) goto encoding_error;
    if(cp > 0x10ffff) goto encoding_error;
    
    if(pc32 != NULL) *pc32 = cp;
    if(cp == 0)
        return 0;
    else
        return nbytes;
encoding_error:
    errno = EILSEQ;
    return (size_t)(-1);
}

size_t c32rtomb(
    char *restrict      s,
    char32_t            c32,
    mbstate_t *restrict ps
) {
    if(s == NULL) {
        *ps = (mbstate_t) {0};
        return 0;
    }
    unsigned cp = c32;
    if(cp >= 0x10ffff) goto encoding_error;
    size_t nbytes = 4;
    if(cp < 0x10000) nbytes = 3;
    if(cp < 0x800)   nbytes = 2;
    if(cp < 0x80)    nbytes = 1;
    switch(nbytes) {
        case 1: {
            s[0] = cp;
        } break;
        case 2: {
            s[0] = 0xc0 | (cp >> 6);
            s[1] = 0x80 | ((cp >> 0)  & 0x3f);
        } break;
        case 3: {
            s[0] = 0xe0 | (cp >> 12);
            s[1] = 0x80 | ((cp >> 6)  & 0x3f);
            s[2] = 0x80 | ((cp >> 0)  & 0x3f);
        } break;
        case 4: {
            s[0] = 0xf0 | (cp >> 18);
            s[1] = 0x80 | ((cp >> 12) & 0x3f);
            s[2] = 0x80 | ((cp >> 6)  & 0x3f);
            s[3] = 0x80 | ((cp >> 0)  & 0x3f);
        } break;
    }
    return nbytes;
encoding_error:
    errno = EILSEQ;
    return (size_t)(-1);
}
