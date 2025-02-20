#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 100

typedef struct {
    double (*matrix)[MAX_SIZE];
    double *b;
    double *x;
    int n;
    int startRow;
    int numRows;
} ThreadData;

void *gauss_elimination(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    double (*matrix)[MAX_SIZE] = data->matrix;
    double *b = data->b;
    int n = data->n;

    for (int i = data->startRow; i < data->startRow + data->numRows; i++) {
        for (int j = i + 1; j < n; j++) {
            double ratio = matrix[j][i] / matrix[i][i];
            for (int k = i; k < n; k++) {
                matrix[j][k] -= ratio * matrix[i][k];
            }
            b[j] -= ratio * b[i];
        }
    }
    return NULL;
}

void back_substitution(double (*matrix)[MAX_SIZE], double *b, double *x, int n) {
    for (int i = n - 1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= matrix[i][j] * x[j];
        }
        x[i] /= matrix[i][i];
    }
}

void solve_linear_system(double (*matrix)[MAX_SIZE], double *b, double *x, int n) {
    int numThreads = 4;
    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];

    for (int i = 0; i < numThreads; i++) {
        threadData[i].matrix = matrix;
        threadData[i].b = b;
        threadData[i].n = n;
        threadData[i].startRow = i * (n / numThreads);
        threadData[i].numRows = (i == numThreads - 1) ? (n - threadData[i].startRow) : (n / numThreads);

        pthread_create(&threads[i], NULL, gauss_elimination, &threadData[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    back_substitution(matrix, b, x, n);
}

void input_matrix(double (*matrix)[MAX_SIZE], double *b, int *n) {
    printf("Введите размерность матрицы (n): ");
    scanf("%d", n);
    
    printf("Введите элементы матрицы (строка за строкой):\n");
    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *n; j++) {
            scanf("%lf", &matrix[i][j]);
        }
    }

    printf("Введите элементы вектора свободных членов (b):\n");
    for (int i = 0; i < *n; i++) {
        scanf("%lf", &b[i]);
    }
}

int main() {
    double matrix[MAX_SIZE][MAX_SIZE];
    double b[MAX_SIZE], x[MAX_SIZE];
    int n;

    input_matrix(matrix, b, &n);
    solve_linear_system(matrix, b, x, n);

    printf("Решение:\n");
    for (int i = 0; i < n; i++) {
        printf("x[%d] = %lf\n", i, x[i]);
    }

    return 0;
}
