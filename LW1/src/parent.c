#include "../include/utils.h"
#include <wait.h>


int haupt() {
    int pipefd[2]; // Массив для хранения дескрипторов pipe
    pid_t pid;
    char buffer[MAX_SIZE];

    // Создание pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Создание дочернего процесса
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        // Родительский процесс
        close(pipefd[0]); // Закрываем дескриптор для чтения

        printf("Введите строку с числами: ");
        fgets(buffer, MAX_SIZE, stdin); // Ввод строки

        // Запись строки в pipe
        write_to_pipe(pipefd[1], buffer);
        close(pipefd[1]); // Закрываем дескриптор для записи

        // Ожидание завершения дочернего процесса
        wait(NULL);
    } else {
        // Дочерний процесс
        process_input_from_pipe(pipefd[0], pipefd[1]);
        exit(0); // Завершение дочернего процесса
    }

    return 0;
}