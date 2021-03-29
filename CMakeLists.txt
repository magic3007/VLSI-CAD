cmake_minimum_required(VERSION 3.0.2)
project(VLSI-CAD LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(3rdparty)

add_subdirectory(src)

enable_testing()
add_subdirectory(unittest)