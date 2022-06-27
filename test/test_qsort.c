
#include <stdlib.h>
#include <stdio.h>

int compare_ints(const void *p, const void *q) {
    int x = *(const int *)p;
    int y = *(const int *)q;

    /* Avoid return x - y, which can cause undefined behaviour
       because of signed integer overflow. */
    if (x < y)
        return -1;  // Return -1 if you want ascending, 1 if you want descending order. 
    else if (x > y)
        return 1;   // Return 1 if you want ascending, -1 if you want descending order.

    return 0;
    // All the logic is often alternatively written:
    return (x > y) - (x < y);
}

/* Sort an array of n integers, pointed to by a. */
void sort_ints(int *a, size_t n) {
    qsort(a, n, sizeof(*a), compare_ints);
}

int main() {
    int arr[] = {-1,10,155, 256-10,0,0,0};
    int count = sizeof arr / sizeof *arr;
    sort_ints(arr, count);
    for(int i = 0; i != count; ++i) {
        printf("%d\t", arr[i]);
    }
}

