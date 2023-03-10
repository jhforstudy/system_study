#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) { // child process
        printf("hello\n");
    }
    else {  // parent process
        // int wc = wait(NULL);
        printf("goodbye\n");
    }
    return 0;
}