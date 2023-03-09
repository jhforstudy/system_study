#include <stdio.h>

int main() {
    unsigned int i;
    int a[3] = {1, 2, 3};
    for (i = sizeof(a)/sizeof(int) - 1; i>=0; i--)
        printf("%d\n", a[i]);
    return 0;
}