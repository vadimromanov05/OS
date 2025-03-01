#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <limits.h>


typedef float (*SinIntegralFunc)(float, float, float);
typedef char* (*TranslationFunc)(long);

int main() {
    void *handle;
    SinIntegralFunc SinIntegral;
    TranslationFunc Translation;
    char *error;
    float A, B, e;
    long x;
    int command;
    int scanf_result;


    handle = dlopen("./libfirst.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }

    SinIntegral = (SinIntegralFunc)dlsym(handle, "SinIntegral_rectangle");
    Translation = (TranslationFunc)dlsym(handle, "translation_2");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        return 1;
    }

    while (1) {
        printf("Enter command:\n 1 for SinIntegral\n 2 for Translation\n 0 to switch\n -1 to exit:\n ");

        scanf_result = scanf("%d", &command);
        if (scanf_result != 1) {
            printf("Invalid command input. Please enter an integer.\n");
            while (getchar() != '\n');
            continue;
        }


        if (command == -1) {
            break;
        } else if (command == 0) {
            dlclose(handle);
            handle = dlopen("./libsecond.so", RTLD_LAZY);
            if (!handle) {
                fprintf(stderr, "%s\n", dlerror());
                return 1;
            }
            SinIntegral = (SinIntegralFunc)dlsym(handle, "SinIntegral_trapezoid");
            Translation = (TranslationFunc)dlsym(handle, "translation_3");
            if ((error = dlerror()) != NULL) {
                fprintf(stderr, "%s\n", error);
                return 1;
            }
            printf("Switched to libsecond.so\n");
        } else if (command == 1) {
            printf("Enter A, B, e: ");
            scanf_result = scanf("%f %f %f", &A, &B, &e);
            if (scanf_result != 3) {
                printf("Invalid input for A, B, or e. Please enter three numbers.\n");
                while (getchar() != '\n');
                continue;
            }
            if (e <= 0 || B <= A) {
                printf("Error: e must be greater than zero and B must be greater than A.\n");
                continue;
            }
            printf("SinIntegral: %f\n", SinIntegral(A, B, e));
        } else if (command == 2) {
            printf("Enter number: ");
            scanf_result = scanf("%ld", &x);
            if (scanf_result != 1) {
                printf("Invalid input. Please enter ein number.\n");
                while (getchar() != '\n');
                continue;
            }
            printf("Translation: %s\n", Translation(x));
        } else {
            printf("Unknown command\n");
        }
    }

    dlclose(handle);
    return 0;
}