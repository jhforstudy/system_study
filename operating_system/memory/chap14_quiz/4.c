#include <stdio.h>
#include <stdlib.h>

int main() {
    int* new_arr = (int*)malloc(sizeof(int) * 5);

    for (int i=0; i<5; i++) {
        new_arr[i] = 0;
        printf("%d ", new_arr[i]);
    }
    printf("\n");
    // free(new_arr);

    return 0;
}