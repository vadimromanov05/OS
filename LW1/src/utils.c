#include "../include/utils.h"


void write_to_pipe(int write_fd, char const *buffer) {
    write(write_fd, buffer, strlen(buffer) + 1);
}
