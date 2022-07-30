
#include <stdio.h>
#include <Windows.h>

#pragma comment(lib, "user32.lib")

int main(int argc, char **argv) {
    // Print args:
    printf("Arguments:\n");
    for(int i = 0; i != argc; ++i) {
        char *arg = argv[i];
        printf("  %s\n", arg);
    }
    // Test UTF-8 support:
    printf(u8"Привет, мир!\n");
    printf(u8"こんにちくわ世界！\n");
    // Try UTF-8 in winapi (doesn't use UTF-8 without manifest)
    MessageBoxA(NULL, u8"Успех", u8"Ошибка", MB_OK);
    return 0;
}
