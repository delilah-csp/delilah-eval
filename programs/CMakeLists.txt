cmake_minimum_required(VERSION 3.16)

project(delilah-eval-programs)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_FLAGS "-target bpf -c -Wno-implicit-function-declaration -O3")

file(GLOB PROGRAMS "*.c")
add_library(programs OBJECT ${PROGRAMS})

foreach(PROGRAM ${PROGRAMS})
  string(REPLACE ".c" "" NAME ${PROGRAM})
  string(REPLACE ${CMAKE_SOURCE_DIR}/programs/ "" NAME ${NAME})
  string(REPLACE "/" "_" TARGET ${NAME})

  add_custom_target(program_${TARGET}
    ALL
    DEPENDS programs
    COMMAND cp "${CMAKE_SOURCE_DIR}/programs/CMakeFiles/programs.dir/${NAME}.c.o" ${NAME}.o
    BYPRODUCTS ${NAME}.o
    COMMENT "Moving ${NAME} object file .. (${NAME}.o)"
  )
endforeach()
