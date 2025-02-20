#ifndef UTILS_H
#define UTILS_H


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


void *gauss_elimination(void *arg);
void back_substitution(double (*matrix)[MAX_SIZE], double *b, double *x, int n);


#endif