
static bool strpfx_i(char const *restrict str, char const *restrict prefix) {
    while(*prefix != 0) {
        if(*str == 0) break;
        if(toupper(*str) != toupper(*prefix)) return false;
        ++prefix;
        ++str;
    }
    return true;
}
