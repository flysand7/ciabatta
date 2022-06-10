#include <stdio.h>
#include <math.h>
#include <fenv.h>
#include <float.h>

#include <emmintrin.h>

#pragma STDC FENV_ACCESS ON

static inline double rint (double const x) {
    return (double)_mm_cvtsd_si32(_mm_load_sd(&x));
}

void show_fe_exceptions(void)
{
    printf("current exceptions raised: ");
    if(fetestexcept(FE_DIVBYZERO))     printf(" FE_DIVBYZERO");
    if(fetestexcept(FE_INEXACT))       printf(" FE_INEXACT");
    if(fetestexcept(FE_INVALID))       printf(" FE_INVALID");
    if(fetestexcept(FE_OVERFLOW))      printf(" FE_OVERFLOW");
    if(fetestexcept(FE_UNDERFLOW))     printf(" FE_UNDERFLOW");
    if(fetestexcept(FE_ALL_EXCEPT)==0) printf(" none");
    printf("\n");
}

void show_fe_current_rounding_direction(void)
{
    printf("current rounding direction:  ");
    switch (fegetround()) {
           case FE_TONEAREST:  printf ("FE_TONEAREST");  break;
           case FE_DOWNWARD:   printf ("FE_DOWNWARD");   break;
           case FE_UPWARD:     printf ("FE_UPWARD");     break;
           case FE_TOWARDZERO: printf ("FE_TOWARDZERO"); break;
           default:            printf ("unknown");
    };
    printf("\n");
}

double x2 (double x)   /* times two */
{
    fenv_t curr_excepts;
 
    /* Save and clear current f-p environment. */
    feholdexcept(&curr_excepts);
 
    /* Raise inexact and overflow exceptions. */
    printf("In x2():  x = %f\n", x=x*2.0);
    show_fe_exceptions();
    feclearexcept(FE_INEXACT);   /* hide inexact exception from caller */
 
    /* Merge caller's exceptions (FE_INVALID)        */
    /* with remaining x2's exceptions (FE_OVERFLOW). */
    feupdateenv(&curr_excepts);
    return x;
}


int main(void)
{
    // ROUNDING MODES
    printf("\nROUNDING MODES\n");
    /* Default rounding direction */
    show_fe_current_rounding_direction();
    printf("+11.5 -> %f\n", rint(+11.5)); /* midway between two integers */
    printf("+12.5 -> %f\n", rint(+12.5)); /* midway between two integers */

    /* Save current rounding direction. */
    int curr_direction = fegetround();

    /* Temporarily change current rounding direction. */
    fesetround(FE_DOWNWARD);
    show_fe_current_rounding_direction();
    printf("+11.5 -> %f\n", rint(+11.5));
    printf("+12.5 -> %f\n", rint(+12.5));

    /* Restore default rounding direction. */
    fesetround(curr_direction);
    show_fe_current_rounding_direction(); 

    // EXCEPTION TEST
    printf("\nEXCEPTIONS\n");

    show_fe_exceptions();

    /* Perform some computations which raise exceptions. */
    printf("1.0/0.0     = %f\n", 1.0/0.0);        /* FE_DIVBYZERO            */
    printf("1.0/10.0    = %f\n", 1.0/10.0);       /* FE_INEXACT              */
    printf("DBL_MAX*2.0 = %f\n", DBL_MAX*2.0);    /* FE_INEXACT FE_OVERFLOW  */
    show_fe_exceptions();

    // FEHOLDEXCEPT
    printf("\nFEHOLDEXCEPT\n");

    feclearexcept(FE_ALL_EXCEPT);
    feraiseexcept(FE_INVALID);   /* some computation with invalid argument */
    show_fe_exceptions();
    printf("x2(DBL_MAX) = %f\n", x2(DBL_MAX));
    show_fe_exceptions();

    return 0;
}
