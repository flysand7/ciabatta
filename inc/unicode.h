
#pragma once

#include <stdint.h>
#include <stddef.h>

typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;

typedef int32_t uchar_t;

#define UNI_EBADCP (-1)
#define UNI_EULSUR (-2)
#define UNI_EIBYTE (-3)
#define UNI_ETBYTE (-4)
#define UNI_ESTRLN (-5)
#define UNI_EOLONG (-6)

enum {
    UCHAR_BAD,
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

enum {
    UCHAR_BIDI_AL,
    UCHAR_BIDI_AN,
    UCHAR_BIDI_B,
    UCHAR_BIDI_BN,
    UCHAR_BIDI_CS,
    UCHAR_BIDI_EN,
    UCHAR_BIDI_ES,
    UCHAR_BIDI_ET,
    UCHAR_BIDI_FSI,
    UCHAR_BIDI_L,
    UCHAR_BIDI_LRE,
    UCHAR_BIDI_LRI,
    UCHAR_BIDI_LRO,
    UCHAR_BIDI_NSM,
    UCHAR_BIDI_ON,
    UCHAR_BIDI_PDF,
    UCHAR_BIDI_PDI,
    UCHAR_BIDI_R,
    UCHAR_BIDI_RLE,
    UCHAR_BIDI_RLI,
    UCHAR_BIDI_RLO,
    UCHAR_BIDI_S,
    UCHAR_BIDI_WS,
};

enum {
    UCHAR_DECOMP_CANON,
    UCHAR_DECOMP_FONT,
    UCHAR_DECOMP_NOBREAK,
    UCHAR_DECOMP_INITIAL,
    UCHAR_DECOMP_MEDIAL,
    UCHAR_DECOMP_FINAL,
    UCHAR_DECOMP_ISOLATED,
    UCHAR_DECOMP_CIRCLE,
    UCHAR_DECOMP_SUPER,
    UCHAR_DECOMP_SUB,
    UCHAR_DECOMP_VERTICAL,
    UCHAR_DECOMP_WIDE,
    UCHAR_DECOMP_NARROW,
    UCHAR_DECOMP_SMALL,
    UCHAR_DECOMP_SQUARE,
    UCHAR_DECOMP_FRACTION,
    UCHAR_DECOMP_COMPAT,
};

typedef struct uchar_props uchar_props;
struct uchar_props {
    uchar_t       code;
    char const   *name;
    int           cat_gen;
    int           cat_bidi;
    int           comb_class;
    int           dec_type;
    int           dec_map_n;
    uchar_t const dec_map[18]; // U+FDFA takes 18, everything else takes up <8
    int           dec_value;
    int           dig_value;
    double        num_value;
    int           bidi_mirrored;
    char const   *old_name;
    char const   *comment;
    uchar_t       lower;
    uchar_t       upper;
    uchar_t       title;
};

uchar_props *uni_props   (uchar_t cp);
int          uni_valid   (uchar_t cp);
int          uni_classify(uchar_t cp);
uchar_t      uni_tolower (uchar_t cp);
uchar_t      uni_toupper (uchar_t cp);
uchar_t      uni_totitle (uchar_t cp);

int     uni_is_hsur(char16_t cp);
int     uni_is_lsur(char16_t cp);
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
