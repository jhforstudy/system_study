#include <stdio.h>
#include <stdlib.h>

int main() {
    int* new_arr = (int*)malloc(sizeof(int) * 100);

    new_arr[50] = 0;

    free(&new_arr[50]);

    printf("%d\n", new_arr[50]);

    return 0;
}
