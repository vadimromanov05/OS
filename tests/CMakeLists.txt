cmake_minimum_required(VERSION 3.10)


include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)


#add_executable(test1 lab1_test.cpp ../LW1/src/parent.c)
#target_link_libraries(test1 PRIVATE GTest::gtest GTest::gtest_main pthread)
#add_test(NAME test1 COMMAND test1)

add_executable(test2 lab2_test.cpp)
target_link_libraries(test2 PRIVATE GTest::gtest GTest::gtest_main pthread)
add_test(NAME test2 COMMAND test2)

add_executable(test3 lab3_test.cpp ../LW3/src/utils.c)
target_link_libraries(test3 PRIVATE GTest::gtest GTest::gtest_main pthread)
add_test(NAME test3 COMMAND test3)

add_executable(test4 lab4_test.cpp ../LW4/src/src1.c ../LW4/src/src2.c)
target_include_directories(test4 PUBLIC ../LW4/include)
target_link_libraries(test4 PRIVATE GTest::gtest GTest::gtest_main pthread)

add_executable(test567 lab567_test.cpp)
target_include_directories(test567 PUBLIC ../LW567/include)
target_link_libraries(test567 PRIVATE lab567_tools lab567_controller lab567_worker GTest::gtest)

add_executable(test_cp lab8_tests.cpp)
target_sources(test_cp PRIVATE ../CP/src/Job.cpp ../CP/src/DAG.cpp ../CP/src/ThreadPool.cpp ../CP/src/Barrier.cpp)
target_include_directories(test_cp PRIVATE ../lab8/include)
target_link_libraries(test_cp PRIVATE GTest::gtest GTest::gtest_main pthread)
