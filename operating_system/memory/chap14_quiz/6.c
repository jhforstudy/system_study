#include <stdio.h>
#include <stdlib.h>

int main() {
    int* new_arr = (int*)malloc(sizeof(int) * 5);

    // allocate integer
    for (int i=0; i<5; i++)
        new_arr[i] = i;
    
    // free
    free(new_arr);
    
    // print
    for (int i=0; i<5; i++)
        printf("%d ", new_arr[i]);
    printf("\n");

    return 0;
}