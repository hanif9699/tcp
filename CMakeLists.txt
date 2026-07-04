cmake_minimum_required(VERSION 3.13)

project(my_project C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
file(GLOB SOURCES app/*.c)

add_executable(my_program ${SOURCES})

target_compile_options(my_program PRIVATE
    -g
    -O0
    -Wall
    -Wextra
    -fsanitize=address
)

target_link_options(my_program PRIVATE
    -fsanitize=address
)