
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

#define F64_BITS(x) ((union {f64 f; u64 i;}){.f=x}).i
#define F64_CONS(x) ((union {f64 f; u64 i;}){.i=x}).f
#define F32_BITS(x) ((union {f32 f; u32 i;}){.f=x}).i
#define F32_CONS(x) ((union {f32 f; u32 i;}){.i=x}).f

#define F64_MANT_MASK UINT64_C(0xfffffffffffff)
#define F64_MANT_MAX  UINT64_C(0xfffffffffffff)
#define F64_MANT_BITS 52
#define F64_BEXP_BITS 11
#define F64_BEXP_MASK 0x7ff
#define F64_BEXP_MAX  0x7ff

#define F32_MANT_MASK 0x7fffff
#define F32_MANT_MAX  0x7fffff
#define F32_MANT_BITS 23
#define F32_BEXP_BITS 8
#define F32_BEXP_MASK 0xff
#define F32_BEXP_MAX  0xff


#define F64_SIGN(bits) (bits >> (F64_MANT_BITS + F64_BEXP_BITS))
#define F64_BEXP(bits) ((bits >> F64_MANT_BITS) & F64_BEXP_MASK)
#define F64_MANT(bits) ((bits) & F64_MANT_MASK)

#define F32_SIGN(bits) (bits >> (F32_MANT_BITS + F32_BEXP_BITS))
#define F32_BEXP(bits) ((bits >> F32_MANT_BITS) & F32_BEXP_MASK)
#define F32_MANT(bits) ((bits) & F32_MANT_MASK)
