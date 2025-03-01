#include <math.h>

#include "../include/lib1.h"


float SinIntegral_rectangle(float A, float B, float e) {
    float sum = 0.0;
    for(float x = A; x < B; x += e) {
        sum += sin(x) * e;
    }
    return sum;
}


char result1[32];


char* translation_2(long x) {
    if (x == 0) {
        if (result1 != NULL) {
            result1[0] = '0';
            result1[1] = '\0';
        }
        return result1;
    }

    long num = x;
    if (x < 0) {
        num = -x;
    }

    char buffer[32];
    int index = 0;
   
    while (num > 0) {
        buffer[index++] = (num % 2) + '0';
        num /= 2;
    }

    if (x < 0) {
        buffer[index++] = '-';
    }

    buffer[index] = '\0';

    if (result1 != NULL) {
        for (int i = 0; i < index; i++) {
            result1[i] = buffer[index - 1 - i];
        }
        result1[index] = '\0';
    }

    return result1;
}