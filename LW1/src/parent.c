#include "../include/utils.h"
#include <wait.h>


int haupt() {
    int pipefd[2];
    pid_t pid;
    char buffer[MAX_SIZE];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        close(pipefd[0]);

        printf("Введите строку с числами: ");
        fgets(buffer, MAX_SIZE, stdin);

        write_to_pipe(pipefd[1], buffer);
        close(pipefd[1]);

        wait(NULL);
    } else {
        process_input_from_pipe(pipefd[0], pipefd[1]);
        exit(0);
    }

    return 0;
}