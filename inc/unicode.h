
#pragma once

#include <stdint.h>
#include <stddef.h>

typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;

typedef int32_t uchar_t;
typedef enum {
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
} uchar_class;

int uni_classify(uchar_t ch);
int uni_valid(uchar_t ch);
uchar_t uni_to_lower(uchar_t u);
uchar_t uni_to_upper(uchar_t u);

int utf8_dec   (char const *restrict utf8_str,  uchar_t *restrict ch);
int utf16_dec  (char const *restrict utf16_str, uchar_t *restrict ch);
int utf8_dec_s (char const *restrict utf8_str,  size_t len, uchar_t *restrict ch);
int utf16_dec_s(char const *restrict utf16_str, size_t len, uchar_t *restrict ch);

int utf8_enc   (char *utf8_str,  uchar_t ch);
int utf16_enc  (char *utf16_str, uchar_t ch);
int utf8_enc_s (char *utf8_str,  size_t len, uchar_t ch);
int utf16_enc_s(char *utf16_str, size_t len, uchar_t ch);
