cmake_minimum_required(VERSION 3.6)
project(neat)

set(CMAKE_C_STANDARD 99)

include_directories(include)
set(SOURCE_FILES src/main.c src/link.c src/list.c src/innovation.c include/genome.h src/genome.c src/neuron.c src/population.c src/species.c)
add_executable(neat ${SOURCE_FILES})
target_link_libraries(neat m)
