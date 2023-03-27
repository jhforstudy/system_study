#include <stdio.h>
#include <stdlib.h>

int main() {
    int* data = (int*)malloc(sizeof(int) * 100);

    data[100] = 0;

    printf("%d\n", data[100]);
    
    free(data);

    return 0;
}