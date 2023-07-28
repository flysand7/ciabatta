
static void _fileapi_init();

void _cia_init() {
    _fileapi_init();
}

void memset(void *dest, int ch, size_t length) {
    u8 *bytes = dest;
    for(int i = 0; i != length; ++i) {
        bytes[i] = ch;
    }
}

int _wcsicmp(uint16_t *s1, uint16_t *s2) {
    while(*s1 != 0 && *s2 != 0) {
        if(*s1 != *s2) {
            return *s1 - *s2;
        }
    }
    if(*s1 == 0 && *s2 != 0) {
        return -1;
    }
    if(*s1 != 0 && *s2 == 0) {
        return 1;
    }
    return 0;
}