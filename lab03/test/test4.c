#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main (void) {
        pid_t pid, ppid;
        uid_t uid;
        pid = getpid();
        ppid = getppid();
        uid = getuid();

        printf("PID %d\n", pid);
        printf("PPID %d\n", ppid);
        printf("PPID %d\n", uid);

        getchar();
return 0;
}