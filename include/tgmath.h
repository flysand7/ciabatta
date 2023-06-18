
#pragma once

#include <math.h>

#define _tg_is_ld(x) (sizeof(x) == sizeof(long double))
#define _tg_is_d(x)  (sizeof(x) == sizeof(double))

#define _tg_is_ld2(x,y) (_tg_is_ld(x) || _tg_is_ld(y))
#define _tg_is_d2(x,y)  (_tg_is_d (x) || _tg_is_d (y))

#define _tg_is_ld3(x,y,z) (_tg_is_ld(x) || _tg_is_ld(y) || _tg_is_ld(z))
#define _tg_is_d3(x,y,z)  (_tg_is_d (x) || _tg_is_d (y) || _tg_is_d (z))


#define _tg_func1(func,x)      ( _tg_is_ld(x)      ? (func ## l)(x) \
                               : _tg_is_d(x)       ?      (func)(x) \
                                                   : (func ## f)(x) )


#define _tg_func1p(func,x,p)   ( _tg_is_ld(x)      ? (func ## l)(x,p) \
                               : _tg_is_d(x)       ?      (func)(x,p) \
                                                   : (func ## f)(x,p) )

#define _tg_func2(func,x,y)    ( _tg_is_ld2(x,y)   ? (func ## l)(x,y) \
                               : _tg_is_d2(x,y)    ?      (func)(x,y) \
                                                   : (func ## f)(x,y) )

#define _tg_func2p(func,x,y,p) ( _tg_is_ld2(x,y)   ? (func ## l)(x,y,p) \
                               : _tg_is_d2(x,y)    ?      (func)(x,y,p) \
                                                   : (func ## f)(x,y,p) )


#define _tg_func3(func,x,y,z)  ( _tg_is_ld3(x,y,z) ? (func ## l)(x,y,z) \
                               : _tg_is_d3(x,y,z)  ?      (func)(x,y,z) \
                                                   : (func ## f)(x,y,z) )

#define cos(x)          _tg_func1(cos,   x)
#define sin(x)          _tg_func1(sin,   x)
#define tan(x)          _tg_func1(tan,   x)
#define acos(x)         _tg_func1(acos,  x)
#define asin(x)         _tg_func1(asin,  x)
#define atan(x)         _tg_func1(atan,  x)
#define atan2(x,y)      _tg_func2(atan2, x, y)

#define cosh(x)         _tg_func1(cosh,  x)
#define sinh(x)         _tg_func1(sinh,  x)
#define tanh(x)         _tg_func1(tanh,  x)
#define acosh(x)        _tg_func1(acosh, x)
#define asinh(x)        _tg_func1(asinh, x)
#define atanh(x)        _tg_func1(atanh, x)

#define exp(x)          _tg_func1(exp,   x)
#define exp2(x)         _tg_func1(exp2,  x)
#define expm1(x)        _tg_func1(expm1, x)
#define pow(x,y)        _tg_func2(pow,   x, y)
#define log(x,y)        _tg_func2(log,   x, y)
#define sqrt(x)         _tg_func1(sqrt,  x)
#define cbrt(x)         _tg_func1(cbrt,  x)
#define hypot(x,y)      _tg_func2(hypot, x, y)

#define log10(x)        _tg_func1(log10, x)
#define log1p(x)        _tg_func1(log1p, x)
#define log2(x)         _tg_func1(log2,  x)
#define logb(x)         _tg_func1(logb,  x)
#define ilogb(x)        _tg_func1(ilogb, x)

#define ceil(x)         _tg_func1(ceil,    x)
#define floor(x)        _tg_func1(floor,   x)
#define trunc(x)        _tg_func1(trunc,   x)
#define llrint(x)       _tg_func1(llrint,  x)
#define lrint(x)        _tg_func1(lrint,   x)
#define rint(x)         _tg_func1(rint,    x)
#define llround(x)      _tg_func1(llround, x)
#define lround(x)       _tg_func1(lround,  x)
#define round(x)        _tg_func1(round,   x)
#define nearbyint(x)    _tg_func1(nearbyint, x)

#define erf(x)          _tg_func1(erf,      x)
#define erfc(x)         _tg_func1(erfc,     x)
#define tgamma(x)       _tg_func1(tgamma,   x)
#define lgamma(x)       _tg_func1(lgamma,   x)

#define fabs(x)         _tg_func1(fabs,      x)
#define fdim(x,y)       _tg_func2(fdim,      x, y)
#define fmax(x,y)       _tg_func2(fmax,      x, y)
#define fmin(x,y)       _tg_func2(fmin,      x, y)
#define fmod(x,y)       _tg_func2(fmod,      x, y)
#define remainder(x,y)  _tg_func2(remainder, x, y)
#define remquo(x,y,p)   _tg_func2p(remquo,   x, y, p)
#define fma(x,y,z)      _tg_func3(fma,       x, y, z)

#define copysign(x,y)   _tg_func2(copysign, x, y)
#define frexp(x,p)      _tg_func1p(frexp,   x, p)
#define ldexp(x,p)      _tg_func1p(ldexp,   x, p)
#define modf(x,p)       _tg_func1p(modf,    x, p)
#define scalbn(x,p)     _tg_func1p(scalbn,  x, p)
#define scalbln(x,p)    _tg_func1p(scalbln, x, p)

#define nextafter(x,y)  _tg_func2(nextafter,  x, y)
#define nexttoward(x,y) _tg_func2(nexttoward, x, y)
