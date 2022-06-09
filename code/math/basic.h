
ftype fabs(ftype x) {
    if(x >= 0) {
        return x;
    }
    return -x;
}

double fmod(double x, double y) {
    int n = 0;
    while(y < x*n) {
        ++n;
    }
    return y - n*x;
}

float fmodf(float x, float y) {
    int n = 0;
    while(y < x*n) {
        ++n;
    }
    return y - n*x;
}


