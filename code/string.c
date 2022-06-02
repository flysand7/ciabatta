
int strcmp(const char *s1, const char *s2)
{
    while(*s1 != 0 && *s2 != 0) {
        if(*s1 != *s2) break;
    }
    if(*s1 > *s2) return 1;
    if(*s1 < *s2) return -1;
    return 0;
}
