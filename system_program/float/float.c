#include <stdio.h>

typedef unsigned char *ptr;

void show_bytes(ptr start, size_t len) {
    size_t i;
    for(i=0; i<len; i++) {
        printf("%p\t%x\n", start+i, start[i]);
    }
    printf("\n");
}

int main() {
    float x = 43.25;
    int y = 152;
    double z = 15234.234234;

    printf("float : %f\n", x);
    show_bytes((ptr) &x, sizeof(float));
    printf("int : %d\n", y);
    show_bytes((ptr) &y, sizeof(int));
    printf("double : %f\n", z);
    show_bytes((ptr) &z, sizeof(double));

    return 0;
}