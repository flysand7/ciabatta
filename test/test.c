#include <assert.h>
#include <ctype.h>

int main(void) {
	for (char c = 'a'; c != 'z'; ++c) {
		assert(isupper(toupper(c)));
    }
    return 0;
}
