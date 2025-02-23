cmake_minimum_required(VERSION 3.10)


include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)


#add_executable(test1 lab1_test.cpp ../LW1/src/utils.c ../LW1/src/parent.c ../LW1/src/child.c ../LW1/include/utils.h)
#target_link_libraries(test1 PRIVATE GTest::gtest GTest::gtest_main pthread)
#add_test(NAME test1 COMMAND test1)

add_executable(test2 lab2_test.cpp)
target_link_libraries(test2 PRIVATE GTest::gtest GTest::gtest_main pthread)
add_test(NAME test2 COMMAND test2)

add_executable(test3 lab3_test.cpp ../LW3/src/utils.c)
target_link_libraries(test3 PRIVATE GTest::gtest GTest::gtest_main pthread)
add_test(NAME test3 COMMAND test3)

add_executable(test4 lab4_test.cpp ../LW4/src/src1.c)
target_include_directories(test4 PUBLIC ../LW4/include)
target_link_libraries(test4 PRIVATE PRIVATE GTest::gtest GTest::gtest_main pthread)

#add_library(MyLibrary SHARED ../LW4/src/src1.c)
#add_executable(test4_2 lab42_test.cpp)
#target_link_libraries(test4_2 PRIVATE GTest::gtest GTest::gtest_main pthread MyLibrary)

#add_executable(test4_1 lab41_test.cpp)
#target_include_directories(test4_1 PUBLIC ../LW4/include)
#target_link_libraries(test4_1 PRIVATE GTest::gtest_main GTest::GTest pthread)