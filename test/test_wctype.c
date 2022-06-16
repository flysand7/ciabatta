
#include <wctype.h>
#include <stdio.h>

int main() {
    if(iswalpha(L'я')) {
        printf("Symbol 'я' is indeed a letter\n");
    }
    return 0;
}
