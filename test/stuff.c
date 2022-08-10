
#include <string.h>
#include <stdio.h>



int main() {
    char arr[] = "Bruh\0\0\0";
    printf("Before memmove: %s\n", &arr[0]);
    memmove(arr+3, arr, 4);
    printf("After memmove: %s\n", &arr[0]);
    return 0;
}
