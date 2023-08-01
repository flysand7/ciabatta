
#pragma once

// Include platform stuff
#include <cia-conf.h>

// Pre-C23 keyword macros and stddef
#define static_assert _Static_assert
#define NULL ((void *)0)

// Assert commonly-accepted platform-invariant sizes
#include <stdint.h>
#include <stdbool.h>
    
// Short type definitions
typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
