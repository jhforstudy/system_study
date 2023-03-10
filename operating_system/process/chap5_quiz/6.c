#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) {
        // int wc = wait(NULL); // what if?
        printf("hello, I am child (pid: %d)\n", (int)getpid());
    }
    else {
        int wc = waitpid(NULL);
        printf("hello, I am parent of %d (wc: %d) (pid: %d)\n", rc, wc, (int)getpid());
    }
    return 0;
}