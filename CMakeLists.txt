cmake_minimum_required(VERSION 3.27)
project(htop C)

target_link_libraries(htop -lncurses)

set(CMAKE_C_STANDARD 11)

add_executable(main.c)
