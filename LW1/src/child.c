#include "../include/utils.h"


void process_input_from_pipe(int read_fd, int pipefd) {
    char buffer[MAX_SIZE];
    close(pipefd);

    read(read_fd, buffer, MAX_SIZE);
    close(read_fd);

    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    printf("Дочерний процесс: Введенная строка: %s\n", buffer);

    char *token = strtok(buffer, " \n");
    if (token != NULL) {
        double first_number = atof(token);
        token = strtok(NULL, " \n");
        //printf("1 is done!\n");

        while (token != NULL) {
            //printf("Iteration!\n");
            double next_number = atof(token);
            
            if (next_number != 0) {
                double result = first_number / next_number;
                fprintf(file, "%f / %f = %f\n", first_number, next_number, result);
            } else {
                fprintf(file, "Деление на ноль невозможно при делении %f / %f.\n", first_number, next_number);
                fclose(file);
                exit(EXIT_FAILURE);
            }

            token = strtok(NULL, " \n");
        }
        //printf("2 is done!\n");
    } else {
        fprintf(file, "Не введено ни одного числа.\n");
    }
    //printf("3 is done!\n");
    fclose(file);
    //printf("4 is done!\n");
}