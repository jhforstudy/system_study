#include <stdio.h>

// 자료형을 재정의함
typedef unsigned char *ptr;

// C언어에서 size_t : 어떤 객체나 값이 포함될 수 있는 최대 사이즈를 표현하는 type
void show_bytes(ptr start, size_t len) {
    size_t i;
    for (i=0; i<len; i++) {
        // %p : pointer 주소를 그대로 print, 포인터가 들어가야 함
        // %x : 해당 주소에 있는 값을 표현, unsigned int
        printf("%p\t0x%x\n", start+i, start[i]);
    }
    printf("\n");
}

int main() {
    int A = 15213;

    printf("int A = 15213;\n");
    show_bytes((ptr) &A, sizeof(int));
    
}