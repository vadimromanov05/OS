find_package(PkgConfig REQUIRED)
pkg_check_modules(YAML_CPP REQUIRED yaml-cpp)

include_directories(${YAML_CPP_INCLUDE_DIRS})
link_directories(${YAML_CPP_LIBRARY_DIRS})

add_library(cp8_utils src/cp8_utils.cpp)
target_include_directories(cp8_utils PRIVATE include)
target_link_libraries(cp8_utils PRIVATE ${YAML_CPP_LIBRARIES})

add_executable(cp8 main.cpp)
target_include_directories(cp8 PRIVATE include)
target_link_libraries(cp8 PRIVATE cp8_utils ${YAML_CPP_LIBRARIES})