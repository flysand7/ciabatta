
int isalnum(int c) {
    return isalpha(c) || isdigit(c);
}

int isalpha(int c) {
    return islower(c) || isupper(c);
}

int isblank(int c) {
    return c == ' ' || c == '\t';
}

int iscntrl(int c) {
    return IN_RANGE('\x00', c, '\x1f') || c == '\x7f';
}

int isdigit(int c) {
    return IN_RANGE('0', c, '9');
}

int isgraph(int c) {
    return isprint(c) && (c != ' ');
}

int islower(int c) {
    return IN_RANGE('a', c, 'z');
}

int isprint(int c) {
    return IN_RANGE(' ', c, '\x7e');
}

int ispunct(int c) {
    return IN_RANGE('\x21', c, '\x2f')
        || IN_RANGE('\x3a', c, '\x40')
        || IN_RANGE('\x5b', c, '\x60')
        || IN_RANGE('\x7b', c, '\x7e');
}

int isspace(int c) {
    return IN_RANGE('\x09', c, '\x0d') || c == ' ';
}

int isupper(int c) {
    return IN_RANGE('A', c, 'Z');
}

int isxdigit(int c) {
    return IN_RANGE('0', c, '9')
        || IN_RANGE('a', c, 'f')
        || IN_RANGE('A', c, 'F');
}

int tolower(int c) {
    if(isupper(c)) {
        return c-'A'+'a';
    }
    return c;
}

int toupper(int c) {
    if(islower(c)) {
        return c-'a'+'A';
    }
    return c;
}
