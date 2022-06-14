cmake_minimum_required(VERSION 3.12.2)
project(delilah-eval)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(programs)

FILE(GLOB_RECURSE FILES hermes.c delilah.c)
add_executable(delilah-eval ${FILES})
