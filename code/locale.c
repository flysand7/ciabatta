
// TODO: X-macros
// TODO: data race possible (see 7.11.1.1 p. 5)
// TODO: something about restoring locales (see 7.11.1.1 p. 8)
#include <locale.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

#define _LC_FIRST       0
#define _LC_LAST        5

static struct lconv _locale;
static const char *_locale_str;

char *setlocale(int category, const char *locale)
{
    assert(_LC_FIRST <= category && category <= _LC_LAST);

    if(locale == NULL) {
        return _locale_str;
    }
    if(strcmp(locale, "C") == 0) {
        if(category == LC_ALL) {
            _locale.decimal_point = ".";
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
        }
    }
    else {
        return NULL;
    }
    _locale_str = locale;
    return locale;
}

struct lconv *localeconv(void)
{

}
