#include <stdio.h>

// union (공용체) : struct와 동일하지만, union은 멤버들의 시작 주소가 모두 동일하다.
union {
    int i;
    unsigned char c[4];
} u;

int main() {
    u.i = 0x12345678;
    // %c : char 형으로 프린트
    // %x : 1byte의 값 표현 (ASCII), unsigned int가 들어가야 함
    printf("%x %x %x %x\n", u.c[0], u.c[1], u.c[2], u.c[3]);
}