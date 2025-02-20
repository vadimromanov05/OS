#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_SIZE 100
#define FILENAME "results.txt"

void process_input_from_pipe(int read_fd, int pipefd);
void write_to_pipe(int write_fd, const char *buffer);
int haupt();

#endif