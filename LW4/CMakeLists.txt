set(CMAKE_C_STANDARD 11)
project(mai-os)

add_library(first SHARED src/src1.c)
target_include_directories(first PRIVATE include)
target_link_libraries(first PUBLIC m)

add_library(second SHARED src/src2.c)

add_executable(lab4_static static.c)
target_include_directories(lab4_static PRIVATE include)
target_link_libraries(lab4_static first second)

add_executable(lab4_dinamic dynamic.c)
target_include_directories(lab4_dinamic PRIVATE include)