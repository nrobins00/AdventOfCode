#include <stdio.h>
#include <stdlib.h>
int main() {
    int *arr = (int *)malloc(10*sizeof(int));
    printf("sizeof(arr): %d\n", sizeof(arr));
    printf("sizeof(&arr): %d\n", sizeof(&arr));
    int size = sizeof(&arr) / sizeof(arr[0]);

    printf("%d\n", size);
}
