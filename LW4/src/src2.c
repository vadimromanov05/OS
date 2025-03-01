#include <stdlib.h>
#include <math.h>


float SinIntegral_trapezoid(float A, float B, float e) {
    float sum = 0.0;
    for(float x = A; x < B; x += e) {
        sum += (sin(x) + sin(x + e)) / 2 * e;
    }
    return sum;
}


char result[32];

char* translation_3(long x) {
    if (x == 0) {
        if (result != NULL) {
            result[0] = '0';
            result[1] = '\0';
        }
        return result;
    }

    long num = x;
    if (x < 0) {
        num = -x;
    }

    char buffer[21];
    int index = 0;
   
    while (num > 0) {
        buffer[index++] = (num % 3) + '0';
        num /= 3;
    }

    if (x < 0) {
        buffer[index++] = '-';
    }

    buffer[index] = '\0';

    if (result != NULL) {
        for (int i = 0; i < index; i++) {
            result[i] = buffer[index - 1 - i];
        }
        result[index] = '\0';
    }

    return result;
}