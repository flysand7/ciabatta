
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <fenv.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>

const char *show_classification(double x) {
    switch(fpclassify(x)) {
        case FP_INFINITE:  return "Inf";
        case FP_NAN:       return "NaN";
        case FP_NORMAL:    return "normal";
        case FP_SUBNORMAL: return "subnormal";
        case FP_ZERO:      return "zero";
        default:           return "unknown";
    }
}

int main() {

    printf("\n=== fpclassify === \n");
    double zero = 0.0; // fucken msvc
    printf("1.0/0.0 is %s\n", show_classification(1.0/zero));
    printf("0.0/0.0 is %s\n", show_classification(0.0/zero));
    printf("DBL_MIN/2 is %s\n", show_classification(DBL_MIN/2));
    printf("-0.0 is %s\n", show_classification(-0.0));
    printf("1.0 is %s\n", show_classification(1.0));

    printf("\n\n=== nearbyint === \n");
    double d;
    printf("nearbyint(+0.0)  = %f\n", d=nearbyint(+0.0));
    printf("nearbyint(-0.0)  = %f\n", d=nearbyint(-0.0));
    printf("nearbyint(+16.4) = %f\n", d=nearbyint(+16.4));
    printf("nearbyint(+16.5) = %f\n", d=nearbyint(+16.5));
    printf("nearbyint(+16.8) = %f\n", d=nearbyint(+16.8));

    printf("\n\n=== signbit === \n");
    printf("signbit(+0.0) = %d\n", signbit(+0.0));
    printf("signbit(-0.0) = %d\n", signbit(-0.0));

    printf("\n\n=== copysign === \n");
    printf("copysign(1.0,+2.0)      = %f\n", copysign(1.0,+2.0));
    printf("copysign(1.0,-2.0)      = %f\n", copysign(1.0,-2.0));
    printf("copysign(INFINITY,-2.0) = %f\n", copysign(INFINITY,-2.0));
    printf("copysign(NAN,-2.0)      = %f\n", copysign(NAN,-2.0));

    printf("\n\n=== rint === \n");
    fesetround(FE_TONEAREST);
    printf("rounding to nearest (halfway cases to even):\n"
           "rint(+2.3) = %f  ", rint(2.3));
    printf("rint(+2.5) = %f  ", rint(2.5));
    printf("rint(+3.5) = %f\n", rint(3.5));
    printf("rint(-2.3) = %f  ", rint(-2.3));
    printf("rint(-2.5) = %f  ", rint(-2.5));
    printf("rint(-3.5) = %f\n", rint(-3.5));
    fesetround(FE_DOWNWARD);
    printf("rounding down: \nrint(+2.3) = %f  ", rint(2.3));
    printf("rint(+2.5) = %f  ", rint(2.5));
    printf("rint(+3.5) = %f\n", rint(3.5));
    printf("rint(-2.3) = %f  ", rint(-2.3));
    printf("rint(-2.5) = %f  ", rint(-2.5));
    printf("rint(-3.5) = %f\n", rint(-3.5));
    feclearexcept(FE_ALL_EXCEPT);
    printf("rint(1.1) = %f\n", rint(1.1));
    if(fetestexcept(FE_INEXACT)) printf("    FE_INEXACT was raised\n");

    printf("\n\n=== nextafter === \n");
    float from1 = 0, to1 = nextafterf(from1, 1);
    printf("The next representable float after %f is %f\n", from1, to1);
    float from2 = 1, to2 = nextafterf(from2, 2);
    printf("The next representable float after %f is %f\n", from2, to2);
    float from5 = 0.0, to5 = nextafter(from5, -0.0);
    printf("nextafter(+0.0, -0.0) gives %f (%f)\n", to5, to5);

    printf("\n\n=== ceil === \n");
    printf("ceil(+2.4) = %f\n", ceil(2.4));
    printf("ceil(-2.4) = %f\n", ceil(-2.4));
    printf("ceil(-0.0) = %f\n", ceil(-0.0));
    printf("ceil(-Inf) = %f\n", ceil(-INFINITY));

    printf("\n\n=== floor === \n");
    printf("floor(+2.7) = %f\n", floor(2.7));
    printf("floor(-2.7) = %f\n", floor(-2.7));
    printf("floor(-0.0) = %f\n", floor(-0.0));
    printf("floor(-Inf) = %f\n", floor(-INFINITY));

    printf("\n\n=== trunk === \n");
    printf("trunc(+2.7) = %f\n", trunc(2.7));
    printf("trunc(-2.7) = %f\n", trunc(-2.7));
    printf("trunc(-0.0) = %f\n", trunc(-0.0));
    printf("trunc(-Inf) = %f\n", trunc(-INFINITY));

    printf("\n\n=== round === \n");
    printf("round(+2.3)  = %f  ",  round(2.3));
    printf("round(+2.5)  = %f  ",  round(2.5));
    printf("round(+2.7)  = %f\n",  round(2.7));
    printf("round(-2.3)  = %f  ",  round(-2.3));
    printf("round(-2.5)  = %f  ",  round(-2.5));
    printf("round(-2.7)  = %f\n",  round(-2.7));
    printf("round(-0.0)  = %f\n",  round(-0.0));
    printf("round(-Inf)  = %f\n",  round(-INFINITY));
    printf("lround(+2.3) = %ld  ", lround(2.3));
    printf("lround(+2.5) = %ld  ", lround(2.5));
    printf("lround(+2.7) = %ld\n", lround(2.7));
    printf("lround(-2.3) = %ld  ", lround(-2.3));
    printf("lround(-2.5) = %ld  ", lround(-2.5));
    printf("lround(-2.7) = %ld\n", lround(-2.7));
    printf("lround(-0.0) = %ld\n", lround(-0.0));
    printf("lround(-Inf) = %ld\n", lround(-INFINITY));
    feclearexcept(FE_ALL_EXCEPT);
    printf("lround(LONG_MAX+1.5) = %ld\n", lround(LONG_MAX+1.5));
    if(fetestexcept(FE_INVALID)) printf("    FE_INVALID was raised\n");

    printf("\n\n=== exp === \n");
    printf("exp(1) = %e\n", exp(1));
    printf("FV of $100, continuously compounded at 3%% for 1 year = %e\n",
            100*exp(0.03));
    // special values
    printf("exp(-0) = %e\n", exp(-0.0));
    printf("exp(-Inf) = %e\n", exp(-INFINITY));
    //error handling
    errno = 0; feclearexcept(FE_ALL_EXCEPT);
    printf("exp(710) = %e\n", exp(710));
    if(errno == ERANGE) printf("    errno == ERANGE\n");
    if(fetestexcept(FE_OVERFLOW)) printf("    FE_OVERFLOW raised\n");

    return 0;
}
