cmake_minimum_required(VERSION 3.6)
project(neat)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c neat.c node.c connection.c genome.c)
add_executable(neat ${SOURCE_FILES})