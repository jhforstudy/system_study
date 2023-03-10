#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) { // child process
        close(STDOUT_FILENO);
        printf("hello\n");
    }
    else {  // parent process
        int wc = wait(NULL);
    }
    return 0;
}