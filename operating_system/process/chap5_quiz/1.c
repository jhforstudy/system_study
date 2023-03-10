#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int x;
    x = 100;

    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) { // child process
        printf("Before (%d) : %d\n", (int)getpid(), x);
        x = 200;
        printf("After child (%d) : %d\n\n", (int)getpid(), x);
    }
    else {  // parent process
        printf("Before (%d) : %d\n", (int)getpid(), x);
        x = 300;
        printf("After parent (%d) : %d\n\n", (int)getpid(), x);
    }
    return 0;
}