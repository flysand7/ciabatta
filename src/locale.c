
#define _LC_FIRST       0
#define _LC_LAST        5

static struct lconv _locale;
static char _locale_str[16];

// Even if the user doesn't enable LIB_EXT1 we still have it existing
size_t strnlen_s(const char *s, size_t maxsize);

char *setlocale(int category, const char *locale) {
    if (_LC_FIRST <= category && category <= _LC_LAST) return NULL;
    if (locale == NULL) return _locale_str;
    
    // Validate the string a bit
    size_t locale_len = strnlen_s(locale, sizeof(_locale_str));
    if (locale_len == 0 || locale_len >= sizeof(_locale_str)) return NULL;
    
    if(strcmp(locale, "C") == 0) {
        switch (category) {
        case LC_ALL: {
            _locale.decimal_point = "."; // fuck the french, the german, the russians and the rest of the wrongthinkers
            _locale.thousands_sep = "";
            _locale.grouping = "";
            _locale.mon_decimal_point = "";
            _locale.mon_thousands_sep = "";
            _locale.mon_grouping = "";
            _locale.positive_sign = "";
            _locale.negative_sign = "";
            _locale.currency_symbol = "";
            _locale.frac_digits = CHAR_MAX;
            _locale.p_cs_precedes = CHAR_MAX;
            _locale.n_cs_precedes = CHAR_MAX;
            _locale.p_sep_by_space = CHAR_MAX;
            _locale.n_sep_by_space = CHAR_MAX;
            _locale.p_sign_posn = CHAR_MAX;
            _locale.n_sign_posn = CHAR_MAX;
            _locale.int_curr_symbol = "";
            _locale.int_frac_digits = CHAR_MAX;
            _locale.int_p_cs_precedes = CHAR_MAX;
            _locale.int_n_cs_precedes = CHAR_MAX;
            _locale.int_p_sep_by_space = CHAR_MAX;
            _locale.int_n_sep_by_space = CHAR_MAX;
            _locale.int_p_sign_posn = CHAR_MAX;
            _locale.int_n_sign_posn = CHAR_MAX;
            break;
        }
        
        default: return NULL;
        }
    } else {
        return NULL;
    }
    
    memcpy(_locale_str, locale, locale_len);
    return _locale_str;
}

struct lconv *localeconv(void) {
    return &_locale;
}
