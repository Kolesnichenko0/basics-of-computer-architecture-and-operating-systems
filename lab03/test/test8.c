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

    while ((opt = getopt(argc, argv, "w:f:")) != -1) {
        switch (opt) {
            case 'w':
                checked = 0;
                exec = optarg;
                break;
            case 'f':
                exec = optarg;
                checked = 1;
                break;
            default:
                printf("Usage: %s -w|-f exec\n", argv[0]);
                exit(1);
        }
    }
    if (exec == NULL) {
        printf("Usage: %s -w|-f exec\n", argv[0]);
        printf("HU\n");
        exit(1);
    }
    if (checked == 0) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork error");
            exit(1);
        } else if (pid == 0) {
            if (execvp(exec, &argv[2]) == -1) {
                perror("Programme execution error\n");
                exit(1);
            }
        } else {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                printf("Process exited with status %d\n", WEXITSTATUS(status));
            }
        }
    } else {
        pid_t pid, pid2;
        if ((pid = fork()) == -1) {
            perror("Fork error\n");
            exit(1);
        } else if (pid == 0) {
            pid2 = fork();
            if (pid2 == -1) {
                perror("Fork error\n");
                exit(1);
            } else if (pid2 == 0) {
                if (execlp(exec, exec, NULL) == -1) {
                    perror("Programme execution error\n");
                    exit(1);
                }
            } else {
                printf("Attempt to run the descendant with pid=%d\n", pid2);
                exit(0);
            }
        } else {
            waitpid(pid, NULL, 0);
            waitpid(pid2, NULL, 0);
        }
    }
    printf("Please press <enter> to exit:");
    getchar();
    exit(0);
}