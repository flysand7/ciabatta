
#include<_platform.h>
#include<locale.h>

extern int main();

int mainCRTStartup()
{
    setlocale(LC_ALL, "C");
    return main();
}
