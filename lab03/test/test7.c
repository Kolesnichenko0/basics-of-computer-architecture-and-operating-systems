#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <getopt.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    char *filename;
    int i, j, async = 0;

    // Поиск ключа -f и имени исполняемого файла
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            filename = argv[i+1];
            break;
        }
    }

    // Асинхронный запуск, если передан ключ -a
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            async = 1;
            break;
        }
    }

    // Запуск процесса с использованием fork и execvp
    pid_t pid = fork();

    if (pid == -1) {
        printf("Ошибка: fork не удался\n");
        return 1;
    } else if (pid == 0) {
        // Дочерний процесс

        // Двойной fork, чтобы избежать появления зомби
        pid_t child_pid = fork();

        if (child_pid == -1) {
            printf("Ошибка: fork не удался\n");
            return 1;
        } else if (child_pid == 0) {
            // Внуковый процесс
            char *args[] = {filename, NULL};
            execvp(filename, args);
            printf("Ошибка: execvp не удался\n");
            return 1;
        } else {
            // Дочерний процесс завершается
            exit(0);
        }
    } else {
        // Родительский процесс

        // Асинхронный запуск
        if (async) {
            printf("Процесс запущен асинхронно\n");
            return 0;
        }

        // Ожидание завершения дочернего процесса
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Процесс завершился с кодом %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Процесс завершился из-за сигнала %d\n", WTERMSIG(status));
        } else {
            printf("Процесс завершился неожиданно\n");
        }
    }

    return 0;
}