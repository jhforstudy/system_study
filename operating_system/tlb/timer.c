#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define PAGE_SIZE 4096
#define ARRAY_SIZE (128 * PAGE_SIZE)

int main() {
    struct timeval start_time, end_time;
    long elapsed_time;

    int *test_arr = (int*)malloc(sizeof(int) * ARRAY_SIZE);

    // 시작 시간
    gettimeofday(&start_time, NULL);

    // array를 page size만큼 이동하며, 임의의 값 추가
    for (int j = 0; j < ARRAY_SIZE; j += PAGE_SIZE) {
        test_arr[j] = 1;
    }

    // 끝나는 시간
    gettimeofday(&end_time, NULL);

    // 경과 시간 계산 (ns)
    elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000000000 + (end_time.tv_usec - start_time.tv_usec);

    printf("Elapsed time: %ld microseconds\n", elapsed_time);

    free(test_arr);

    return 0;
}
