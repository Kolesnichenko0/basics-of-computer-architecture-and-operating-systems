#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: %s <L> <U>\n", argv[0]);
        exit(1);
    }
    int L = atoi(argv[1]);
    int U = atoi(argv[2]);
    int sum = 0;
    int prod = 1;

    for(int i = L; i <= U; i++) {
        sum += i;
        prod *= i;
    }

    printf("Sum: %d\nProduct: %d\n", sum, prod);

    return 0;

}