
#include <ctype.h>

#define in_range(low, v, high) ((low <= v) && (v <= high))

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
    return in_range('\x00', c, '\x1f') || c == '\x7f';
}

int isdigit(int c) {
    return in_range('0', c, '9');
}

int isgraph(int c) {
    return isprint(c) && (c != ' ');
}

int islower(int c) {
    return in_range('a', c, 'z');
}

int isprint(int c) {
    return in_range(' ', c, '\x7e');
}

int ispunct(int c) {
    return in_range('\x21', c, '\x2f')
        || in_range('\x3a', c, '\x40')
        || in_range('\x5b', c, '\x60')
        || in_range('\x7b', c, '\x7e');
}

int isspace(int c) {
    return in_range('\x09', c, '\x0d') || c == ' ';
}

int isupper(int c) {
    return in_range('A', c, 'Z');
}

int isxdigit(int c) {
    return in_range('0', c, '9')
        || in_range('a', c, 'f')
        || in_range('A', c, 'F');
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
