#include <stdio.h>
#include <math.h>
#include <fenv.h>

#include <emmintrin.h>

#pragma STDC FENV_ACCESS ON

static inline double rint (double const x) {
    return (double)_mm_cvtsd_si32(_mm_load_sd(&x));
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
 
int main(void)
{
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
 
    return 0;
}
