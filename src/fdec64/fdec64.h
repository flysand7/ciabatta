
#pragma once

typedef struct fdec64 {
    uint64_t mantissa;
    int32_t exponent;
} fdec64;

fdec64 dtofdec64(const uint64_t ieeeMant, const uint32_t ieeeExp);
