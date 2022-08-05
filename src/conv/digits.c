
static inline bool isbase(int c, int base) {
    int val = 0;
    if(isdigit(c)) {
        val = c-'0';
    }
    else if(islower(c)) {
        val = c-'a'+10;
    }
    else if(isupper(c)) {
        val = c-'A'+10;
    }
    else {
        return false;
    }
    return val < base;
}

static inline intl todigit(int c) {
    int val = 0;
    if(isdigit(c)) {
        val = c-'0';
    }
    else if(islower(c)) {
        val = c-'a'+10;
    }
    else if(isupper(c)) {
        val = c-'A'+10;
    }
    return val;
}

