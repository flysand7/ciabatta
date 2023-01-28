
double fabs(double x) {
    union {double f; uint64_t i;} u = {x};
    u.i &= -1ULL/2;
    return u.f;
}

float fabsf(float x) {
    union {float f; uint32_t i;} u = {x};
    u.i &= 0x7fffffff;
    return u.f;
}

long double fabsl(long double x) {
    return fabs(x);
}

double fdim(double x, double y) {
    if (isnan(x)) return x;
    if (isnan(y)) return y;
    return x > y ? x - y : 0;
}

float fdimf(float x, float y) {
    if (isnan(x)) return x;
    if (isnan(y)) return y;
    return x > y ? x - y : 0;
}

long double fdiml(long double x, long double y) {
    return fdim(x, y);
}

double fmax(double x, double y) {
    if (isnan(x)) return y;
    if (isnan(y)) return x;
    /* handle signed zeros, see C99 Annex F.9.9.2 */
    if (signbit(x) != signbit(y))
        return signbit(x) ? y : x;
    return x < y ? y : x;
}

float fmaxf(float x, float y) {
    if (isnan(x)) return y;
    if (isnan(y)) return x;
    /* handle signed zeroes, see C99 Annex F.9.9.2 */
    if (signbit(x) != signbit(y))
        return signbit(x) ? y : x;
    return x < y ? y : x;
}

long double fmaxl(long double x, long double y) {
    return fmax(x, y);
}

double fmin(double x, double y) {
    if (isnan(x)) return y;
    if (isnan(y)) return x;
    /* handle signed zeros, see C99 Annex F.9.9.2 */
    if (signbit(x) != signbit(y))
        return signbit(x) ? x : y;
    return x < y ? x : y;
}

float fminf(float x, float y) {
    if (isnan(x)) return y;
    if (isnan(y)) return x;
    /* handle signed zeros, see C99 Annex F.9.9.2 */
    if (signbit(x) != signbit(y))
        return signbit(x) ? x : y;
    return x < y ? x : y;
}

long double fminl(long double x, long double y) {
    return fmin(x, y);
}

#ifdef __CUIK__
#warning "Cuik doesn't support the FMA intrinsics... fix that NeGate"
double fma(double x, double y, double z) {
    return (x * y) + z;
}

float fmaf(float x, float y, float z) {
    return (x * y) + z;
}

long double fmal(long double x, long double y, long double z) {
    return (x * y) + z;
}

double sqrt(double x) {
    return 0.0;
}

float sqrtf(float x) {
    return 0.0;
}
#else
double fma(double x, double y, double z) {
    __m128d xd = _mm_set_sd(x);
    __m128d yd = _mm_set_sd(y);
    __m128d zd = _mm_set_sd(z);
    __m128d rd = _mm_fmadd_sd(xd, yd, zd);
    double res = _mm_cvtsd_f64(rd);
    return res;
}

float fmaf(float x, float y, float z) {
    __m128 xd = _mm_set_ss(x);
    __m128 yd = _mm_set_ss(y);
    __m128 zd = _mm_set_ss(z);
    __m128 rd = _mm_fmadd_ss(xd, yd, zd);
    float res = _mm_cvtss_f32(rd);
    return res;
}

long double fmal(long double x, long double y, long double z) {
    return fma(x, y, z);
}

double sqrt(double x) {
    __m128d xd = _mm_set_sd(x);
    __m128d yd = _mm_set_sd(0);
    __m128d rd = _mm_sqrt_sd(xd, yd);
    double res = _mm_cvtsd_f64(rd);
    return res;
}

float sqrtf(float x) {
    __m128 xd = _mm_set_ss(x);
    __m128 rd = _mm_sqrt_ss(xd);
    float res = _mm_cvtss_f32(rd);
    return res;
}

long double sqrtl(long double x) {
    return sqrt(x);
}
#endif
