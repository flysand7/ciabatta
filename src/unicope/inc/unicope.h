
#pragma once

#include <stdint.h>
#include <stddef.h>

typedef unsigned char  char8_t;
typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;

#define UNI_EBADCP (-1)
#define UNI_EULSUR (-2)
#define UNI_EIBYTE (-3)
#define UNI_ETBYTE (-4)
#define UNI_ESTRLN (-5)
#define UNI_EOLONG (-6)

#define UCHAR_MAJOR_CAT_MASK 0x38
#define UCHAR_MINOR_CAT_MASK 0x07
#define UCHAR_CAT_GEN_MASK   0x3f
#define UCHAR_MIRR_BIT       0x40

#define UCHAR_C 0x08
#define UCHAR_L 0x10 
#define UCHAR_M 0x18
#define UCHAR_N 0x20
#define UCHAR_P 0x28
#define UCHAR_S 0x30
#define UCHAR_Z 0x38

enum Unicode_General_Category {
    UCHAR_Cc = UCHAR_C | 0x00,
    UCHAR_Cf = UCHAR_C | 0x01,
    UCHAR_Co = UCHAR_C | 0x02,
    UCHAR_Cs = UCHAR_C | 0x03,
    UCHAR_Ll = UCHAR_L | 0x00,
    UCHAR_Lm = UCHAR_L | 0x01,
    UCHAR_Lo = UCHAR_L | 0x02,
    UCHAR_Lt = UCHAR_L | 0x03,
    UCHAR_Lu = UCHAR_L | 0x04,
    UCHAR_Mc = UCHAR_M | 0x00,
    UCHAR_Me = UCHAR_M | 0x01,
    UCHAR_Mn = UCHAR_M | 0x02,
    UCHAR_Nd = UCHAR_N | 0x00,
    UCHAR_Nl = UCHAR_N | 0x01,
    UCHAR_No = UCHAR_N | 0x02,
    UCHAR_Pc = UCHAR_P | 0x00,
    UCHAR_Pd = UCHAR_P | 0x01,
    UCHAR_Pe = UCHAR_P | 0x02,
    UCHAR_Pf = UCHAR_P | 0x03,
    UCHAR_Pi = UCHAR_P | 0x04,
    UCHAR_Po = UCHAR_P | 0x05,
    UCHAR_Ps = UCHAR_P | 0x06,
    UCHAR_Sc = UCHAR_S | 0x00,
    UCHAR_Sk = UCHAR_S | 0x01,
    UCHAR_Sm = UCHAR_S | 0x02,
    UCHAR_So = UCHAR_S | 0x03,
    UCHAR_Zl = UCHAR_Z | 0x00,
    UCHAR_Zp = UCHAR_Z | 0x01,
    UCHAR_Zs = UCHAR_Z | 0x02,
};

enum Unicode_Bidi_Class {
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

enum Unicode_Decomposition {
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

// Character functions
char32_t const *uni_dec_map (char32_t cp, int *num);
int             uni_valid   (char32_t cp);
int             uni_classify(char32_t cp);
char32_t        uni_tolower (char32_t cp);
char32_t        uni_toupper (char32_t cp);
char32_t        uni_totitle (char32_t cp);
int             uni_is_hsur (char16_t cp);
int             uni_is_lsur (char16_t cp);
char32_t        uni_surtoc  (char16_t hsur, char16_t lsur);

// UTF-16 encoding
int utf16_chlen(char16_t const *s);
int utf16_chdec(char16_t const *restrict s, size_t len, char32_t *restrict c);
int utf16_chenc(char16_t *s, size_t len, char32_t c);

// UTF-8 encoding
int utf8_chlen(char8_t const *s);
int utf8_chdec(char8_t const *restrict s, size_t len, char32_t *restrict c);
int utf8_chenc(char8_t *s, size_t len, char32_t c);
