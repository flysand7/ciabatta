
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

// TODO: won't work until %a is implemented
int main(int argc, char **argv) {
    double lo = -1;
    double hi = 1;
    double step = 0.0001;
    FILE *out = fopen("bin/data.csv", "w");
    for(double i = lo; i < hi; i += step) {
        fprintf(out, "%f", i);
        fprintf(out, ", ");
    }
    fprintf(out, "\n");
    for(double i = lo; i < hi; i += step) {
        fprintf(out, "%a", asin(i));
        fprintf(out, ", ");
    }
    fclose(out);
    return 0;
}
