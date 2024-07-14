#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SOCK_NAME "socket.soc"
#define BUF_SIZE 256

int main() {
    struct sockaddr_in srvr_addr; // Server address structure
    char buf[BUF_SIZE]; // Buffer to store user input
    int sock; //Socket descriptor

    //printf("Try to connect to socket...\n");
    sock = socket(AF_INET, SOCK_DGRAM, 0); //Create a datagram socket
    if (sock < 0) {
        perror("socket failed");
        return EXIT_FAILURE;
    }

    //Set address family to IPv4
    srvr_addr.sin_family = AF_INET;
    srvr_addr.sin_port = htons(12345); //Set port number to 12345
    //Convert the string IP address to binary form and store it in srvr_addr.sin_addr
    if (inet_aton("127.0.0.1", &srvr_addr.sin_addr) == 0) {
        fprintf(stderr, "inet_aton failed\n");
        close(sock);
        return EXIT_FAILURE;
    }

    while (1) {
        printf("Enter lower and upper bounds (L U): ");
        fgets(buf, sizeof(buf), stdin); //Read user input from the console

        //Send the user input to the server
        if (sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&srvr_addr, sizeof(srvr_addr)) < 0) {
            perror("sendto failed");
            close(sock);
            return EXIT_FAILURE;
        }
        if (buf[0] == 'q') {
            printf("Exiting client...\n");
            break;
        }
    }

    close(sock); //Close the socket
    return 0;
}