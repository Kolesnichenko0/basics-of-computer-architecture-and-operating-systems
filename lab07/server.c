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

//Function to calculate the sum of numbers from L to U
int calculate_sum(int L, int U) {
    int sum = 0;
    for (int i = L; i <= U; ++i) {
        sum += i;
    }
    return sum;
}

//Function to calculate the product of numbers from L to U
int calculate_product(int L, int U) {
    int product = 1;
    for (int i = L; i <= U; ++i) {
        product *= i;
    }
    return product;
}

int main() {
    struct sockaddr_in srvr_addr, client_addr; //Server and client address structures
    char buf[BUF_SIZE]; //Buffer to store received data
    int sock; //Socket descriptor
    int bytes;
    socklen_t client_len; //Length of the client address

    printf("Create socket...\n");
    //Create a datagram socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket failed");
        return EXIT_FAILURE;
    }

    memset(&srvr_addr, 0, sizeof(srvr_addr)); //Initialize server address structure
    srvr_addr.sin_family = AF_INET; //Set address family to IPv4
    srvr_addr.sin_addr.s_addr = htonl(INADDR_ANY); //Bind to any available network interface
    srvr_addr.sin_port = htons(12345); //Set port number to 12345

    printf("Binding to socket...\n");
    //Bind the socket to the server address
    if (bind(sock, (struct sockaddr*)&srvr_addr, sizeof(srvr_addr)) < 0) {
        perror("bind failed");
        close(sock);
        return EXIT_FAILURE;
    }

    while (1) {
        printf("Waiting for data...\n");
        //Receive data from the client.
        bytes = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&client_addr, &client_len);
        if (bytes < 0) {
            perror("recvfrom failed");
            close(sock);
            return EXIT_FAILURE;
        }
        buf[bytes] = '\0'; //Null-terminate the received data

        if (buf[0] == 'q') {
            printf("Exiting server...\n");
            break;
        }

        int L, U;
        if (sscanf(buf, "%d %d", &L, &U) != 2) {
            printf("Invalid data format: %s\n", buf);
            continue;
        }

        if (L > U) {
            printf("Lower bound is greater than upper bound: L=%d, U=%d\n", L, U);
            continue;
        }

        int sum = calculate_sum(L, U);
        int product = calculate_product(L, U);
        //Calculate the sum and product
        printf("Sum: %d\n", sum);
        printf("Product: %d\n", product);
    }

    close(sock); //Close the socket
    return 0;
}