#include <stdio.h>

#include "../include/lib1.h"

typedef enum {
    EXIT = -1,
    INTEGRAl_RECTANGLE = 1,
    INTEGRAL_TRAPEZOID = 2,
    CONVERT_TO_2 = 3,
    CONVERT_TO_3 = 4
} Command;

int main(void) {
    printf(
        "Usage:\n\t\b-1 - exit\n"
        "\t1 - find INTEGRAl_RECTANGLE\n\t2 - find INTEGRAL_TRAPEZOID\n"
        "\t3 - find CONVERT_TO_2\n\t4 - find CONVERT_TO_3\n"
    );

    while (1) {
        printf("command> ");
        int command;
        scanf("%d", &command);

        if (command == EXIT) {
            return 0;
        }

        if (command == 1) {
            float A, B, e;
            printf("Insert value(-s):\n");
            scanf(" %f %f %f", &A, &B, &e);
            const float result = SinIntegral_rectangle(A, B, e);
            printf("%f\n", result);
        } else if (command == 2) {
            float A, B, e;
            printf("Insert value(-s):\n");
            scanf(" %f %f %f", &A, &B, &e);
            const float result = SinIntegral_trapezoid(A, B, e);
            printf("%f\n", result);
        } else if (command == 3){
            long x;
            printf("Insert value(-s):\n");
            scanf(" %ld", &x);
            char *result = translation_2(x);
            printf("%s\n", result);
        } else if (command == 4){
            long x;
            printf("Insert value(-s):\n");
            scanf(" %ld", &x);
            char *result = translation_3(x);
            printf("%s\n", result);
        }
    }
}