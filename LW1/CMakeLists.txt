set(CMAKE_C_STANDARD 11)
project(lab1)

set(CMAKE_C_STANDARD 17)

include_directories(include)

add_executable(lab1 main.c src/parent.c src/child.c src/utils.c include/utils.h)

#g++ lab1_test.cpp ../LW1/src/child.c ../LW1/src/parent.c ../LW1/src/utils.c -o test1 -lgtest -lgmock -pthread
#gcc parent.c child.c main.c -o program