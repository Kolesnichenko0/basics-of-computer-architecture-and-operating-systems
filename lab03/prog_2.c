#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char *argv[]) {
    pid_t pid = getpid(); //идентификатор процесса
    pid_t ppid = getppid(); //идентификатор предка
    pid_t sid = getsid(pid); //идентификатор текущей сессии
    pid_t pgid = getpgid(pid); //идентификатор группы процессов

    uid_t uid = getuid(); //идентификатор пользователя, под которым процесс запущен
    gid_t gid = getgid(); //идентификатор группы этого пользователя
    struct passwd *passwd_info = getpwuid(uid);
    struct group *group_info = getgrgid(gid);

    printf("Arguments:\n");
    for (int i = 1; i < argc; i++) {
        printf("%s\t", argv[i]);
    }
    printf("\n");

    printf("process ID: %d\n", pid);
    printf("session ID: %d\n", sid);
    printf("process group ID: %d\n", pgid);
    printf("parent process ID: %d\n", ppid);
    printf("User: %s\n", passwd_info->pw_name);
    printf("Group: %s\n", group_info->gr_name);

    return 0;
}