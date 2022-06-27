
#pragma once

#include <math.h>

#define _tg_is_ld(x) (sizeof(x) == sizeof(long double))
#define _tg_is_d (x) (sizeof(x) == sizeof(double))

#define _tg_is_ld2(x,y) (_tg_is_ld(x) || _tg_is_ld(y))
#define _tg_is_d2 (x,y) (_tg_is_d (x) || _tg_is_d (y))

#define _tg_func1(func, x)    ( _tg_is_ld(x) ? (func ## l)(x) \
                              : _tg_is_d(x)  ? (func)(x)      \
                                             : (func ## f)(x) )

#define _tf_func2(func, x, t) ( _tg_is_ld2(x,y) ? (func ## l)(x,y) \
                              : _tg_is_d2(x,y)  ? (func)(x,y)      \
                                                : (func ## f)(x,y) )

#define cos   (x)   _tg_func1(cos,   x)
#define sin   (x)   _tg_func1(sin,   x)
#define tan   (x)   _tg_func1(tan,   x)
#define acos  (x)   _tg_func1(acos,  x)
#define asin  (x)   _tg_func1(asin,  x)
#define atan  (x)   _tg_func1(atan,  x)
#define atan2 (x,y) _tg_func2(atan2, x, y)

#define cosh  (x)   _tg_func1(cosh,  x)
#define sinh  (x)   _tg_func1(sinh,  x)
#define tanh  (x)   _tg_func1(tanh,  x)
#define acosh (x)   _tg_func1(acosh, x)
#define asinh (x)   _tg_func1(asinh, x)
#define atanh (x)   _tg_func1(atanh, x)

#define exp   (x)   _tg_func1(exp,   x)
#define exp2  (x)   _tg_func1(exp2,  x)
#define pow   (x,y) _tg_func2(pow,   x, y)
#define log   (x,y) _tg_func2(log,   x, y)
#define sqrt  (x)   _tg_func1(sqrt,  x)
#define cbrt  (x)   _tg_func1(cbrt,  x)
#define fabs  (x)   _tg_func1(fabs,  x)

#define ceil  (x)   _tg_func1(ceil,  x)
#define floor (x)   _tg_func1(floor, x)
#define round (x)   _tg_func1(round, x)
#define trunc (x)   _tg_func1(trunc, x)

#define copysign(x,y) _tg_func2(copysign, x, y)

#define fmax  (x,y) _tg_func2(fmax, x, y)
#define fmin  (x,y) _tg_func2(fmin, x, y)
#define fmod  (x,y) _tg_func2(fmod, x, y)

/*
erf
erfc
exp2
expm1
fdim
fma
frexp
hypot
ilogb
ldexp
lgamma
llrint
llround
log10
log1p
log2
logb
lrint
lround
nearbyint
nextafter
nexttoward
remainder
remquo
rint
scalbn
scalbln
tgamma
carg
cimag
conj
cproj
creal
*/