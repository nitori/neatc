cmake_minimum_required(VERSION 3.6)
project(neat)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c neat.c link.c genome.c vector.c node.c species.c population.c list.c)
add_executable(neat ${SOURCE_FILES})
target_link_libraries(neat m)
