#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void newSem(shared_data *shm) {
    memset(shm->data, 0, sizeof(shm->data));
    if (sem_init(&shm->sem_parent, 1, 0) == -1) {
        perror("Failed to initialize parent semaphore");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&shm->sem_child, 1, 0) == -1) {
        perror("Failed to initialize child semaphore");
        exit(EXIT_FAILURE);
    }
}

void setData(shared_data *shm, char* str) {
    strncpy(shm->data, str, sizeof(shm->data) - 1);
    shm->data[sizeof(shm->data) - 1] = '\0';
}

char* readCommand(FILE* stream) {
    const int MAX_SIZE = 256;
    char* buffer = (char*)malloc(MAX_SIZE);
    if (!buffer) {
        printf("Couldn't allocate buffer");
        exit(EXIT_FAILURE);
    }
    if (fgets(buffer, MAX_SIZE, stream) == NULL) {
        free(buffer);
        return NULL;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    if (strlen(buffer) == 0) {
        free(buffer);
        return NULL;
    }
    
    return buffer;
}

void destroy(shared_data *shm) {
    sem_destroy(&shm->sem_parent);
    sem_destroy(&shm->sem_child);
}