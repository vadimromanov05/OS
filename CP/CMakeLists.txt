set(CMAKE_C_STANDARD 11)
project(mai-os)

include(FetchContent)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/benhoyt/inih.git
        GIT_TAG        v3.11.2
)

FetchContent_MakeAvailable(inih)

find_package(PkgConfig REQUIRED)
add_library(cp8_utils src/tools.cpp)
target_include_directories(cp8_utils PRIVATE include)
target_link_libraries(cp8_utils PRIVATE ${YAML_CPP_LIBRARIES})

add_executable(cp8 main.cpp)
target_include_directories(cp8 PRIVATE include)
target_link_libraries(cp8 PRIVATE cp8_utils ${YAML_CPP_LIBRARIES})