
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <fenv.h>

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


    float from1 = 0, to1 = nextafterf(from1, 1);
    printf("The next representable float after %f is %f\n", from1, to1);
    float from2 = 1, to2 = nextafterf(from2, 2);
    printf("The next representable float after %f is %f\n", from2, to2);
    {
        #pragma STDC FENV_ACCESS ON
        feclearexcept(FE_ALL_EXCEPT);
        double from4 = DBL_MAX, to4 = nextafter(from4, INFINITY);
        printf("The next representable double after %f is %f\n",
               from4, to4);
        if(fetestexcept(FE_OVERFLOW)) printf("   raised FE_OVERFLOW\n");
        if(fetestexcept(FE_INEXACT))  printf("   raised FE_INEXACT\n");
    }
    float from5 = 0.0, to5 = nextafter(from5, -0.0);
    printf("nextafter(+0.0, -0.0) gives %f (%f)\n", to5, to5);

    return 0;
}
