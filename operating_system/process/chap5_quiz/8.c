#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int fd[2];

    if (pipe(fd) == -1) { // make a new pipe
        fprintf(stderr, "pipe failed\n");
        exit(1);
    }

    int rc_1 = fork();

    if (rc_1 < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc_1 == 0) { // child_1 process
        printf("Child 1 (%d)\n", (int)getpid());
        close(fd[0]);   // close output pipe fd[0]
        dup2(fd[1], STDOUT_FILENO);  // connect STDOUT to input pipe fd[1]
    }
    else {  // parent process
        int wc_1 = waitpid(rc_1, NULL);
    }

    int rc_2 = fork();

    if (rc_2 < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc_2 == 0) { // child_2 process
        printf("Child 2 (%d)\n", (int)getpid());
        close(fd[1]);   // close input pipe fd[1]
        dup2(fd[0], STDIN_FILENO);  // open input pipe as rc_1
    }
    else {  // parent process
        int wc_2 = waitpid(rc_2, NULL);
    }
    return 0;
}