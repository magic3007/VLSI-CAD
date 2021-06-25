cmake_minimum_required(VERSION 3.10)

project(rctree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES src/rctree-simplified.*)

# student exercise 
add_executable(${PROJECT_NAME} ${SOURCES} src/rctree-exercise.cpp)
