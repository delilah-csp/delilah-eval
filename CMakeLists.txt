cmake_minimum_required(VERSION 3.12.2)
project(delilah-eval)

set(CMAKE_BUILD_TYPE Release)

add_subdirectory(libfort)
add_subdirectory(programs)

FILE(GLOB_RECURSE FILES experiments/*.c)
add_executable(delilah-eval ${FILES} delilah.c)

target_link_libraries(delilah-eval PUBLIC uring-ffi)
target_link_libraries(delilah-eval PRIVATE fort)