#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PAGE_SIZE 4096
#define NUM_PAGES 16384

int main(int argc, char** argv) {
    struct timespec start_time, end_time;
    long elapsed_time;

    // input 받기
    int page_touch = atoi(argv[1]);
    int n_trials = atoi(argv[2]);

    int *test_arr = (int*)malloc(PAGE_SIZE * NUM_PAGES);
    int jump = PAGE_SIZE / sizeof(int);

    // array 초기화
    for (int i = 0; i < NUM_PAGES * jump; i += jump) {
        test_arr[i] = 0;
    }

    for (int i = 0; i < n_trials; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start_time);
        for (int j = 0; j < page_touch * jump; j += jump) {
            test_arr[j] += 1;
            test_arr[j] *= 2;
            test_arr[j] *= 4;
            test_arr[j] *= 8;
            test_arr[j] *= 16;
            test_arr[j] *= 32;
            test_arr[j] /= 32;
            test_arr[j] /= 16;
            test_arr[j] /= 8;
            test_arr[j] /= 4;
            test_arr[j] /= 2;
        }
        clock_gettime(CLOCK_MONOTONIC, &end_time);
        elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000000000 + (end_time.tv_nsec - start_time.tv_nsec);
        // printf("Elapsed time: %ld nanoseconds\n", elapsed_time / page_touch );
        printf("%ld\n", elapsed_time / page_touch );
    }

    free(test_arr);
    return 0;
}