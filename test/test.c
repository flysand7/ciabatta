// MSVC won't use my <assert.h> unless i provide path explicitly
// idk
#include"..\inc\assert.h"
int main()
{
    assert(0);
    return 0;
}