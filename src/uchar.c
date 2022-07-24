
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
        return 0;
    }
    char32_t code_point;
    int mblen = utf8_chdec((char8_t *)s, n, &code_point);
    if(mblen == UNI_ESTRLN) return (size_t)(-2);
    if(mblen <= 0) {
        errno = EILSEQ;
        return (size_t)(-1);
    }
    *pc32 = code_point;
    if(code_point == 0) return 0;
    return (size_t)mblen;
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
    int mblen = utf8_chenc((char8_t *)s, 4, c32);
    if(mblen <= 0) {
        errno = EILSEQ;
        return (size_t)(-1);
    }
    return (size_t)mblen;
}
