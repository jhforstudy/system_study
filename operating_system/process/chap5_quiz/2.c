#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]) {
    char *par_str = "this is parent!\n";
    char *chi_str = "this is child!\n";
    int fd = open( "./a.txt", O_WRONLY | O_CREAT | O_EXCL, 0644);
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) { // child process
        printf("write from child process (%d)\n", (int)getpid());
        write(fd, chi_str, strlen(chi_str));
        printf("close from child process (%d)\n\n", (int)getpid());
        close(fd);
    }
    else {  // parent process
        printf("write from parent process (%d)\n", (int)getpid());
        write(fd, par_str, strlen(par_str));
        printf("close from parent process (%d)\n\n", (int)getpid());
        close(fd);
    }
    return 0;
}