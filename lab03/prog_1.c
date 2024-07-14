#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <getopt.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    char *exec = NULL;
    int opt;
    int status;
    int checked = 0;
    //Розбір аргументів командного рядка за допомогою getopt()
    while ((opt = getopt(argc, argv, "w:f:")) != -1) {
        switch (opt) {
            case 'w':
                exec = optarg;
                checked = 0;
                break;
            case 'f':
                exec = optarg;
                checked = 1;
                break;
            default:
                printf( "Usage: %s -w|-f exec\n", argv[0]);
                exit(1);
        }
    }
    if(exec == NULL) {
        printf( "Usage: %s -w|-f exec\n", argv[0]);
        exit(1);
    }
    if(checked == 0) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork error");
            exit(1);
        } else if (pid == 0) { //потомок
            if (execvp(exec, &argv[2]) == -1) {
                perror("Programme execution error");
                exit(1);
            }
        } else {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) { //предок
                printf("Process exited with status %d\n", WEXITSTATUS(status));
            }
        }
    }
    else {
        pid_t pid, pid2;
        if ((pid = fork()) == -1) {
            perror("Fork error");
            exit(1);
        }
        else if (pid == 0) { //потомок
            pid2 = fork();
            if (pid2 == -1) {
                perror("Fork error");
                exit(1);
            }
            else if(pid2 == 0) { //потомок
                if (execvp(exec, &argv[2]) == -1) {
                    perror("Programme execution error");
                    exit(1);
                }
            }
            else { //предок
                printf("Attempt to run the descendant with pid=%d\n", pid2); // потомка
                exit(0);
            }
        }
        else { //предок
            waitpid(pid, NULL, 0);
        }
    }
    sleep(1);
    printf("Please press <enter> to exit:\n");
    getchar();
    return 0;
}