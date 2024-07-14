#include "header.h"

int main() {
    const double x = -15.2;
    const double y = 4.64;
    const double z = 21.3;
    double A = function_A(x, y);
    double B = function_B(z);
    printf("A = %f\nB = %f \n", A, B);
    return 0;
}
