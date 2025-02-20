cmake_minimum_required(VERSION 3.10)

project(lab3)

set(CMAKE_C_STANDARD 17)

include_directories(include)

add_executable(lab3_exe src/parent.c src/utils.c)
#add_executable(child3 src/child.c src/utils.c)

#include_directories(include)
#add_executable(lab3 main.c src/parent.c src/child.c src/utils.c include/utils.h)
#add_executable(lab3 src/parent.c)
#add_executable(lab3 src/parent.c src/utils.c)
#add_executable(child3 src/child.c src/utils.c)


#gcc parent.c utils.c -o lab3 -lrt