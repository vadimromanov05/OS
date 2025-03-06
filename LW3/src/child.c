#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>
#include "../include/utils.h"


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Necessary arguments were not provided\n");
        exit(EXIT_FAILURE);
    }

    int shm_fd = shm_open(argv[1], O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Failed to open shared_memory\n");
        exit(EXIT_FAILURE);
    }
    shared_data *shm = mmap(NULL, sizeof(shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED) {
        perror("Failed to map to the address space (mmap)\n");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    FILE *output_file = fopen(argv[2], "w");
    if (!output_file) {
        perror("Failed to open output file\n");
        munmap(shm, sizeof(shared_data));
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    int is_error = 0; 

    while (1) {
        sem_wait(&shm->sem_child);

        if (strcmp(shm->data, "EXIT") == 0) {
            break;
        }

        char* token;
        char* rest = shm->data;
        double first_num;
        double result;
        int is_first = 1;
        is_error = 0;

        while ((token = strtok_r(rest, " ", &rest))) {
            char* endptr;
            double num = strtod(token, &endptr);

            if (*endptr != '\0') {
                fprintf(stderr, "Error: invalid input\n");
                is_error = 1;
                break;
            }

            if (is_first) {
                first_num = num;
                result = first_num;
                is_first = 0;
            } else {
                if (num == 0.0) {
                    fprintf(stderr, "Error: Division by zero\n");
                    is_error = 1;
                    break;
                }
                result /= num;
            }
        }
        fprintf(output_file, "Result: %.2f\n", result);
        fflush(output_file);
        if (is_error) {
            setData(shm, "ERROR");
            
        } else {
            //fprintf(output_file, "Result: %.2f\n", result);
            //fflush(output_file);
        }

        sem_post(&shm->sem_parent);
    }

    fclose(output_file);

    destroy(shm);

    munmap(shm, sizeof(shared_data));
    close(shm_fd);

    return is_error ? EXIT_FAILURE : EXIT_SUCCESS;
}