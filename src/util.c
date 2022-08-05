
// Some of these more horizontal screen space than I consider healthy, that's
// why I define shorter versions for some of the types. long integers are
// special offenders

// The other part of this file are the convenience macros that could be used
// mostly anywhere in the ciabatta implementation.

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef int_least8_t  li8;
typedef int_least16_t li16;
typedef int_least32_t li32;
typedef int_least64_t li64;

typedef int_fast8_t   fi8;
typedef int_fast16_t  fi16;
typedef int_fast32_t  fi32;
typedef int_fast64_t  fi64;

typedef long int               intl;
typedef long long int          intll;
typedef unsigned int           intu;
typedef unsigned long int      intul;
typedef unsigned long long int intull;

typedef float       f32;
typedef double      f64;
typedef long double fl64;

typedef wchar_t wchar;

#define IN_RANGE(start, v, end) ((start) <= (v) && (v) <= (end))

#define CONCAT(a,b) a ## b
#define STR_(a) #a
#define STR(a) STR_(a)

#define DOUBLE_BITS(x) ((union {double f; u64 i;}){x}).i
#define DOUBLE_CONS(x) ((union {double f; u64 i;}){x}).f
#define FLOAT_BITS(x)  ((union {float f;  u32 i;}){x}).i
#define FLOAT_CONS(x)  ((union {float f;  u32 i;}){x}).f

