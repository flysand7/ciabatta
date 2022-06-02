
#include <assert.h>
#include <ctype.h>
int main()
{
    for(char c = 'a'; c != 'z'; ++c) {
        assert(isupper(toupper(c)));
    }
    return 0;
}