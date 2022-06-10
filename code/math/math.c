
#include <math.h>
#include <stdint.h>

// Used for float comparisons in ieee754.h
enum Ordering {
    LS,
    EQ,
    GR,
    UN,
} typedef Ordering;

#define ftype   float
#define itype   int32_t
#define f_ebits 8
#define f_mbits 23
#define suffix(n) n ## f
#include "ieee754.h"
#include "pow.h"
#undef suffix
#undef f_mbits
#undef f_ebits
#undef itype
#undef ftype

#define ftype   double
#define itype   int64_t
#define f_ebits 11
#define f_mbits 52
#define suffix(n) n
#include "ieee754.h"
#include "pow.h"
#undef suffix
#undef f_mbits
#undef f_ebits
#undef itype
#undef ftype

_Static_assert(sizeof(long double) == sizeof(double),
    "Are these 'long doubles' in the same room with us right now?");

#define ftype   long double
#define itype   int64_t
#define f_ebits 11
#define f_mbits 52
#define suffix(n) n ## l
#include "ieee754.h"
#include "pow.h"
#undef suffix
#undef f_mbits
#undef f_ebits
#undef itype
#undef ftype
