#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char *argv[]) {
    //Checking for pid in command line arguments
    if (argc != 2) {
        printf("Usage: %s <PID>\n", argv[0]);
        exit(0);
    }

    pid_t pid = atoi(argv[1]);

    //Displaying a message about the possibility of selecting a signal
    printf("Choose a signal to send to process %d:\n", pid);
    printf("1. SIGINT\n");
    printf("2. SIGUSR1\n");

    //User selection of the signal
    int choice;
    scanf("%d", &choice);
    if (choice != 1 && choice != 2) {
        printf("Invalid choice\n");
        exit(0);
    }

    //Sending a process signal
    int signal;
    if (choice == 1) {
        signal = SIGINT;
    } else {
        signal = SIGUSR1;
    }
    if (kill(pid, signal) == -1) {
        perror("kill error");
        exit(0);
    }

    return 0;
}