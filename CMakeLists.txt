cmake_minimum_required(VERSION 3.10)
project(rooky)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(rooky  src/main.cpp src/utils.cpp src/board.cpp src/search/search.cpp src/movegen/movegen.cpp src/pieces.cpp )

target_compile_options(rooky PUBLIC -Ofast -Wall -Werror)