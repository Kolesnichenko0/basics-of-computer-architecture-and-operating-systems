#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int status;
    pid_t pid = fork();
    char *exec = argv[1];
    if (pid == -1) {
        perror("Fork error");
        exit(1);
    }
    else if(pid == 0) { //потомок
        if ((execv(exec, &argv[1])) == -1) {
            perror("Execlp error");
            exit(1);
        }
    }
    else { //предок
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) { //предок
            printf("Process exited with status %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}