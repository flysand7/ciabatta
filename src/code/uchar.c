
#include <uchar.h>
#include <errno.h>

#include <unicope.h>


size_t mbrtoc16(
    char16_t   *restrict pc16,
    char const *restrict s,
    size_t               n,
    mbstate_t  *restrict ps
) {
    // Figure out the conversion state
    static mbstate_t static_mbstate = {0};
    if(ps == NULL) ps = &static_mbstate;
    if(s == NULL) {
        *ps = (mbstate_t) {0xd800};
        return 0;
    }
    // Check leftovers, using 0xd800 as "no leftover" marker because it
    // doesn't encode a valid character.
    if(ps->leftover == 0xd800) {
        // Decode the UTF-8 encoded codepoint
        char32_t code_point;
        int mblen = utf8_chdec((char8_t *)s, n, &code_point);
        if(mblen == UNI_ESTRLN) return (size_t)(-2);
        if(mblen <= 0) goto invalid_seq;
        // Encode the codepoint into UTF-16 string
        char16_t str[2];
        int c16len = utf16_chenc(str, 2, code_point);
        if(c16len <= 0) goto invalid_seq;
        // Assign the decoded UTF-16 character, decide leftover
        if(pc16 != NULL) *pc16 = str[0];
        ps->leftover = (c16len == 2? str[1] : 0xd800);
        return (size_t)mblen;
    }
    else {
        // Otherwise use and reset the leftover
        if(pc16 != NULL) *pc16 = ps->leftover;
        ps->leftover = 0xd800;
        return (size_t)(-3);
    }
invalid_seq:
    errno = EILSEQ;
    return (size_t)(-1);
}



size_t c16rtomb(
    char *restrict      s,
    char16_t            c16,
    mbstate_t *restrict ps
) {
    // Figure out conversion state
    static mbstate_t static_mbstate = {0};
    if(ps == NULL) ps = &static_mbstate;
    if(s == NULL) {
        *ps = (mbstate_t) {0xd800};
        return 0;
    }
    char32_t codepoint_to_write;
    // Check whether a high surrogate was detected in a previous call to the
    // function. If not, the high_surrogate value is 0xd800
    if(ps->high_surrogate == 0xd800) {
        // If c16 is a surrogate record it, or throw an error
        if(uni_is_hsur(c16)) {
            ps->high_surrogate = c16;
            return 0;
        }
        else if(uni_is_lsur(c16)) {
            goto invalid_char;
        }
        // We'll just write c16
        codepoint_to_write = c16;
    }
    // If high surrogate exists, the next character must be a low surrogate
    // so we'll write a codepoint made out of high and low surrogates
    else if(uni_is_lsur(c16)) {
        codepoint_to_write = uni_surtoc(ps->high_surrogate, c16);
    }
    else goto invalid_char;
    // Write the codepoint that we decided to write to multibyte string
    int written_len = utf8_chenc((char8_t *)s, 4, codepoint_to_write);
    if(written_len < 0) {
        goto invalid_char;
    }
    s[written_len] = 0;
    return (size_t)written_len;
invalid_char:
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
