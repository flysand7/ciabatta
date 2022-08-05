
#pragma once

typedef struct {
    uint64_t mantissa;
    int32_t exponent;
} decfloat_t;

decfloat_t todecfloat(const uint64_t ieeeMant, const uint32_t ieeeExp);
