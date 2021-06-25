cmake_minimum_required(VERSION 3.10)

project(timer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES src/io.cpp src/main.cpp src/timer.hpp)

# student exercise 
add_executable(${PROJECT_NAME} ${SOURCES} src/timer-exercise.cpp)
