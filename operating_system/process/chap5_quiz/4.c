#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void exec_ls(void);

int main(int argc, char *argv[]) {
    int rc = fork();
    
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) { // child process
        exec_ls();
    }
    else {  // parent process
        exec_ls();
    }
    return 0;
}

void exec_ls() {
    // parameter input
    char *abs_path = "/bin/ls";
    char *program_name = "ls";
    char *arg_1 = "-al";
    char *arg_2 = "/home";
    char *argv[] = {program_name, arg_1, arg_2, NULL};

    // 해당 경로에 있는 프로그램만 실행
    execl("/bin/ls", "/bin/ls", NULL);
    printf("cannot use execl\n");

    // PATH에 있는 폴더의 모든 프로그램 실행
    // execlp("ls", "ls", NULL);
    // printf("\ncannot use execlp\n");

    // 해당 경로에 있는 + parameter
    // execle("/bin/ls", "/bin/ls", "-al", "/home", NULL);
    // printf("\ncannot use execle\n");
    
    // 아래는 동일하며, 인수에 배열 자체를 집어넣을 수 있음
    
    // 해당 경로에 있는 프로그램만 실행
    // execv("/bin/ls", argv);
    // printf("\ncannot use execv\n");

    // PATH에 있는 폴더의 모든 프로그램 실행
    // execvp("ls", argv);
    // printf("\ncannot use execvp\n");

    // 해당 경로에 있는 + parameter
    // execvpe("/bin/ls", argv, NULL);
    // printf("\ncannot use execvpe\n");
}