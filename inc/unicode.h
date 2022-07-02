
#pragma once

#include <stdint.h>
#include <stddef.h>

typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;

typedef int32_t uchar_t;

enum {
    UCHAR_Invalid,
    UCHAR_Cc,
    UCHAR_Cf,
    UCHAR_Co,
    UCHAR_Cs,
    UCHAR_Ll,
    UCHAR_Lm,
    UCHAR_Lo,
    UCHAR_Lt,
    UCHAR_Lu,
    UCHAR_Mc,
    UCHAR_Me,
    UCHAR_Mn,
    UCHAR_Nd,
    UCHAR_Nl,
    UCHAR_No,
    UCHAR_Pc,
    UCHAR_Pd,
    UCHAR_Pe,
    UCHAR_Pf,
    UCHAR_Pi,
    UCHAR_Po,
    UCHAR_Ps,
    UCHAR_Sc,
    UCHAR_Sk,
    UCHAR_Sm,
    UCHAR_So,
    UCHAR_Zl,
    UCHAR_Zp,
    UCHAR_Zs,
};

typedef struct uchar_props uchar_props;
struct uchar_props {
    int     bidi_class;
    int     bidi_mirrored;
    int     bidi_paired_bracket;
    int     bidi_paired_bracket_type;
    int     block;
    int     canon_comb_class;
    uchar_t ch_lower;
    uchar_t ch_upper;
    int     ndecomp;
    uchar_t const decomp[4];
    uchar_t default_igncp;
    int     deprecated;
    int     east_asian_width;
    int     gcat;
    int     hangul_syl_type;
    int     join_type;
    int     join_group;
    int     line_brk;
    char    const *name;
    uchar_t nc_cp;
    int     num_val;
    int     ws;
    int     dash;
    int     letter_props;
    int     math_props;
    int     script;
};


int uni_classify(uchar_t ch);
int uni_valid(uchar_t ch);
uchar_t uni_to_lower(uchar_t u);
uchar_t uni_to_upper(uchar_t u);

int     uni_is_hsur(char16_t ch);
int     uni_is_lsur(char16_t ch);
uchar_t uni_surtoc (char16_t hsur, char16_t lsur);

int utf16_chlen(char16_t const *str);
int utf8_chlen (char     const *str);

int utf16_dec_s(char16_t const *restrict str, size_t len, uchar_t *restrict ch);
int utf8_dec_s (char     const *restrict str, size_t len, uchar_t *restrict ch);
int utf16_dec  (char16_t const *restrict str,             uchar_t *restrict ch);
int utf8_dec   (char     const *restrict str,             uchar_t *restrict ch);

int utf16_enc_s(char16_t *str, size_t len, uchar_t ch);
int utf8_enc_s (char     *str, size_t len, uchar_t ch);
int utf16_enc  (char16_t *str,             uchar_t ch);
int utf8_enc   (char     *str,             uchar_t ch);
