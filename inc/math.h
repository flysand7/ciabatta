
#pragma once

typedef float float_t;
typedef double double_t;

#ifndef _HUGE_ENUF
#define _HUGE_ENUF  1e+300  // _HUGE_ENUF*_HUGE_ENUF must overflow
#endif

#define INFINITY   ((float)(_HUGE_ENUF * _HUGE_ENUF))
#define HUGE_VAL   ((double)INFINITY)
#define HUGE_VALF  ((float)INFINITY)

#define NAN        (-(float)(INFINITY * 0.0F))

// FP_ILOGB0
// FP_ILOGBNAN

#define MATH_ERRNO     1
#define MATH_ERREXCEPT 2

#define math_errhandling MATH_ERRNO

// Classification
#define FP_INFINITE  0
#define FP_NAN       1
#define FP_NORMAL    2
#define FP_SUBNORMAL 3
#define FP_ZERO      4
int     _fpclassify(double f);
int     _fpclassifyf(float f);
#define fpclassify(x) (sizeof(x)==4?_fpclassifyf(x):_fpclassify(x))
#define isfinite(x)   (fpclassify(x) != FP_INFINITE && fpclassify(x) != FP_NAN)
#define isinf(x)      (fpclassify(x) == FP_INFINITE)
#define isnan(x)      (fpclassify(x) == FP_NAN)
#define isnormal(x)   (fpclassify(x) == FP_NORMAL)

// Sign bit shit
int     _signbit(double f);
int     _signbitf(float f);
#define signbit(x)    (sizeof(x)==4?_signbitf(x):signbit(x))
float   copysignf(float x, float y);

// Ordering
#define isgreater(x)      (sizeof(x)==4?_isgrtf(x):_isgrt(x))
#define isgreaterequal(x) (sizeof(x)==4?_isgeqf(x):_isgeq(x))
#define isless(x)         (sizeof(x)==4?_islesf(x):_isles(x))
#define islessequal(x)    (sizeof(x)==4?_isleqf(x):_isleq(x))
#define islessgreater(x)  (sizeof(x)==4?_islegf(x):_isleg(x))
#define isunordered(x)    (sizeof(x)==4?_isunof(x):_isuno(x))

double  acos(double x);
float   acosf(float x);

double asin(double x);
float asinf(float x);

double atan(double x);
float atanf(float x);

double atan2(double y, double x);
float atan2f(float y, float x);

double cos(double x);
float cosf(float x);

double sin(double x);
float sinf(float x);

double tan(double x);
float tanf(float x);

double acosh(double x);
float acoshf(float x);

double asinh(double x);
float asinhf(float x);

double atanh(double x);
float atanhf(float x);

double cosh(double x);
float coshf(float x);

double sinh(double x);
float sinhf(float x);

double tanh(double x);
float tanhf(float x);

double exp(double x);
float expf(float x);

double exp2(double x);
float exp2f(float x);

double expm1(double x);
float expm1f(float x);

double frexp(double value, int *exp);
float frexpf(float value, int *exp);

int ilogb(double x);
int ilogbf(float x);

double ldexp(double x, int exp);
float ldexpf(float x, int exp);

double log(double x);
float logf(float x);

double log10(double x);
float log10f(float x);

double log1p(double x);
float log1pf(float x);

double log2(double x);
float log2f(float x);

double logb(double x);
float logbf(float x);

double modf(double value, double *iptr);
float modff(float value, float *iptr);

double scalbn(double x, int n);
float scalbnf(float x, int n);

double scalbln(double x, long int n);
float scalblnf(float x, long int n);

double cbrt(double x);
float cbrtf(float x);

double fabs(double x);
float fabsf(float x);

double hypot(double x, double y);
float hypotf(float x, float y);

double pow(double x, double y);
float powf(float x, float y);

double sqrt(double x);
float sqrtf(float x);

double erf(double x);
float erff(float x);

double erfc(double x);
float erfcf(float x);

double lgamma(double x);
float lgammaf(float x);

double tgamma(double x);
float tgammaf(float x);

double ceil(double x);
float ceilf(float x);

double floor(double x);
float floorf(float x);

double nearbyint(double x);
float nearbyintf(float x);

double rint(double x);
float rintf(float x);

long int lrint(double x);
long int lrintf(float x);

long long int llrint(double x);
long long int llrintf(float x);

double round(double x);
float roundf(float x);

long int lround(double x);
long int lroundf(float x);

long long int llround(double x);
long long int llroundf(float x);

double trunc(double x);
float truncf(float x);

double fmod(double x, double y);
float fmodf(float x, float y);

double remainder(double x, double y);
float remainderf(float x, float y);

double remquo(double x, double y, int *quo);
float remquof(float x, float y, int *quo);

double copysign(double x, double y);
float copysignf(float x, float y);

double nan(const char *tagp);
float nanf(const char *tagp);

double nextafter(double x, double y);
float nextafterf(float x, float y);

double fdim(double x, double y);
float fdimf(float x, float y);

double fmax(double x, double y);
float fmaxf(float x, float y);

double fmin(double x, double y);
float fminf(float x, float y);

double fma(double x, double y, double z);
float fmaf(float x, float y, float z);

