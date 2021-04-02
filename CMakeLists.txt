cmake_minimum_required(VERSION 3.0.2)
project(VLSI-CAD LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# ABC Solver
# Credit to https://github.com/berkeley-abc/abc
set(READLINE_FOUND "FALSE")

add_subdirectory(3rdparty)

add_subdirectory(src)

enable_testing()
add_subdirectory(unittest)