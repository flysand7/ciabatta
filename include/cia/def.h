
#pragma once

// Pre-C23 keyword macros
#define static_assert _Static_assert
    
// Stdint and a layer on top
#include <stdint.h>

// Short type definitions
typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;

// Other commonly-used standard includes
#include <stdbool.h>
#include <stddef.h>
