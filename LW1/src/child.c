#include "../include/utils.h"


void process_input_from_pipe(int read_fd, int pipefd) {
    char buffer[MAX_SIZE];
    close(pipefd); // Закрываем дескриптор для записи

    // Чтение строки из pipe
    read(read_fd, buffer, MAX_SIZE);
    close(read_fd); // Закрываем дескриптор для чтения

    // Открытие файла для записи результатов
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    printf("Дочерний процесс: Введенная строка: %s\n", buffer);

    // Разделение строки на числа
    char *token = strtok(buffer, " \n"); // Первый токен
    if (token != NULL) {
        double first_number = atof(token); // Преобразуем первый токен в число
        token = strtok(NULL, " \n"); // Переходим к следующему токену
        //printf("1 is done!\n");

        // Запись результатов в файл
        while (token != NULL) {
            //printf("Iteration!\n");
            double next_number = atof(token); // Преобразуем следующий токен в число
            
            // Проверяем деление на ноль
            if (next_number != 0) {
                double result = first_number / next_number;
                fprintf(file, "%f / %f = %f\n", first_number, next_number, result);
            } else {
                fprintf(file, "Деление на ноль невозможно при делении %f / %f.\n", first_number, next_number);
                fclose(file); // Закрываем файл перед выходом
                exit(EXIT_FAILURE); // Прекращаем работу программы
            }

            token = strtok(NULL, " \n"); // Переходим к следующему токену
        }
        //printf("2 is done!\n");
    } else {
        fprintf(file, "Не введено ни одного числа.\n");
    }
    //printf("3 is done!\n");
    fclose(file); // Закрываем файл
    //printf("4 is done!\n");
}