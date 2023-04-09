#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PAGE_SIZE 4096
#define NUM_PAGES 1024

int main() {
    struct timespec start_time, end_time;
    long elapsed_time;

    int *test_arr = (int*)malloc(PAGE_SIZE * NUM_PAGES);
    int jump = PAGE_SIZE / sizeof(int);
    
    // array 초기화
    for (int i = 0; i < NUM_PAGES * jump; i += jump) {
        test_arr[i] = 0;
    }

    // 시작 시간 측정
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // array를 jump만큼 이동하며, 임의의 값 추가
    for (int i = 0; i < NUM_PAGES * jump; i += jump) {
        test_arr[i] += 1;
        test_arr[i] *= 2;
        test_arr[i] *= 4;
        test_arr[i] *= 8;
        test_arr[i] *= 16;
        test_arr[i] /= 16;
        test_arr[i] /= 8;
        test_arr[i] /= 4;
        test_arr[i] /= 2;
    }

    // 끝나는 시간 측정
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    // 경과 시간 계산 (ns)
    elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000000000 + (end_time.tv_nsec - start_time.tv_nsec);

    printf("Elapsed time: %ld nanoseconds\n", elapsed_time / NUM_PAGES);

    free(test_arr);

    return 0;
}