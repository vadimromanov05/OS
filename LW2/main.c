#include "../include/utils.h"
#define N 4


int main() {
    double A[N][N + 1];
    A[0][0] = 1; A[0][1] = 1; A[0][2] = 1; A[0][3] = 1; // 2x + 3y + z = 1
    A[1][0] = 0; A[1][1] = 1; A[1][2] = 1; A[1][3] = 1; // 4y + 8z = 2
    A[2][0] = 0; A[2][1] = 2; A[2][2] = 2; A[2][3] = 2; // 3x + 4y + z = 1
    A[3][0] = 0; A[3][1] = 0; A[3][2] = 2; A[3][3] = 2; // x + y + z = 1

    gaussian_elimination(A);

    return 0;
}