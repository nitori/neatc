cmake_minimum_required(VERSION 3.6)
project(neat)

set(CMAKE_C_STANDARD 99)

include_directories(include)
set(SOURCE_FILES src/main.c src/link.c src/list.c src/innovation.c)
add_executable(neat ${SOURCE_FILES})
target_link_libraries(neat m)
