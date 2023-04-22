#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MyStruct {
    char my_str[10];
    int my_int[2];
} MyStruct;

void PrintStruct(MyStruct m) {
    printf("%d %d\n", m.my_int[0], m.my_int[1]);
    printf("%s\n", m.my_str);
}

int main() {
    MyStruct temp;
    temp.my_int[0] = 1;
    temp.my_int[1] = 2;
    strcpy(temp.my_str, "hello world!!!!");

    PrintStruct(temp);

    return 0;
}