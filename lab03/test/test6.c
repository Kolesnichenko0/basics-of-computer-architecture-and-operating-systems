#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <getopt.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    char *exec = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "f:")) != -1) {
        switch (opt) {
            case 'f':
                exec = optarg;
                break;
            default:
                printf( "Usage: %s -f exec\n", argv[0]);
                exit(1);
        }
    }
    if(exec == NULL) {
        printf( "Usage: %s -f exec\n", argv[0]);
        exit(1);
    }
    pid_t pid, pid2;
    if ((pid = fork()) == -1) {
        perror("Fork error\n");
        exit(1);
    }
    else if (pid == 0) {
        pid2 = fork();
        if (pid2 == -1) {
            perror("Fork error\n");
            exit(1);
        }
        else if(pid2 == 0) {
            if (execvp(exec, &argv[2]) == -1) {
                perror("Programme execution error\n");
                exit(1);
            }
        }
        else {
            printf("Attempt to run the descendant with pid=%d\n", pid2); // потомка
            exit(0);
        }
    }
    else {
        waitpid(pid, NULL, 0);
        waitpid(pid2, NULL, 0);
        exit(0);
    }
    //int status;
    //if(WIFEXITED(status)) {
    //    printf("Process exited with status %d\n", WEXITSTATUS(status));
    //}
    printf("Please press <enter> to exit:");
    getchar();

//    int status;
//    do {
//        waitpid(pid2, &status, WNOHANG);
//        if(WIFEXITED(status))
//            sleep(1);
//            break;
//
//    } while(1);
//    printf("Please press <enter> to exit:");
  //  getchar();
    return 0;
}