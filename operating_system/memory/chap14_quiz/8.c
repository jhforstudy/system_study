#include <stdio.h>
#include <stdlib.h>

typedef struct vector {
    int* data;
    int size;
    int capacity;
} vector;

void* vector_init(vector* vec) {
    vec->data = (int*)malloc(sizeof(int) * 10);
    vec->size = 0;
    vec->capacity = 10;

    return vec;
}

void vector_add(vector* vec, int new_val) {
    // vector의 size가 다 차면
    if (vec->size == vec->capacity) {
        vec->capacity *= 2;    // size를 2배 늘리고
        vec->data = (int*)realloc(vec->data, sizeof(int) * vec->capacity);
    }
    vec->data[vec->size++] = new_val;
}

void vector_print(vector* vec) {
    printf("Print vector\n");
    for (int i=0; i<vec->size; i++)
        printf("v[%d] : %d\n", i, vec->data[i]);
    printf("\n");
}

void vector_free(vector* vec) {
    free(vec->data);
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
}

int main() {
    vector v;
    vector_init(&v);

    printf("Allocate vector\n");
    for (int i=0; i<100; i++) {
        vector_add(&v, i);
        printf("%d -> v[%d]\n", i, i);
    }

    vector_print(&v);

    vector_free(&v);

    return 0;
}
