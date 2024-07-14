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
    char buf[256];

    printf("Type L and U (for exit type q) for sending to server: ");
    fflush(stdout);

    //Read the entered data from the console until the 'q' character is received
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        if (buf[0] == 'q') {
            break;
        }
        //Read the values of L and U from the entered string
        if (sscanf(buf, "%d%d", &L, &U) != 2) {
            fprintf(stderr, "Invalid input. Enter again:\n");
            continue;
        }

        //Open the FIFO file to write data
        fd = open(FIFO_NAME, O_WRONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        //Write L to the FIFO file
        if (write(fd, &L, sizeof(L)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        //Write U to the FIFO file
        if (write(fd, &U, sizeof(U)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        //Close the file
        close(fd);

        printf("Type L and U (for exit type q) for sending to server: ");
        //Clears the output buffer for the standard stdout output.
        fflush(stdout);
    }

    //If the user has entered 'q', then we write special values -1
    //for L and U to the FIFO file, which means "end of work"
    L = -1;
    U = -1;


    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (write(fd, &L, sizeof(L)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    if (write(fd, &U, sizeof(U)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    //Close the file
    close(fd);

    return 0;
}
