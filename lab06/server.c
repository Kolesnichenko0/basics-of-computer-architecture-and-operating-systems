#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_NAME "./fifofile"

int main()
{
    int fd;
    int L, U;

    // Creating a named pipe
    if (mkfifo(FIFO_NAME, 0600) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    printf("Server started. Waiting for client...\n");
    while (1) {
        fd = open(FIFO_NAME, O_RDONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Reading L from the client
        if (read(fd, &L, sizeof(L)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // Reading U from the client
        if (read(fd, &U, sizeof(U)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        close(fd);

        //Checking if the client sent the termination signal
        if (L == -1 && U == -1) {
            break;
        }

        // Checking the validity of the input
        if (L >= U) {
            printf("Invalid input. L should be less than U.\n");
            continue;
        }

        // Processing the received range of numbers
        int sum = 0;
        int prod = 1;
        for (int i = L; i <= U; i++) {
            sum += i;
            prod *= i;
        }

        //Displaying the result
        printf("The range [%d;%d] is received from the client. Sum of elements: %d, Element product: %d\n",L, U ,sum, prod);
    }

    // Removing the named pipe
    unlink(FIFO_NAME);

    return 0;
}