project(lab8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(SOURCES
    src/Job.cpp
    src/DAG.cpp
    src/ThreadPool.cpp
    src/Barrier.cpp
    main.cpp
)

include_directories(include)

add_executable(lab8 ${SOURCES})