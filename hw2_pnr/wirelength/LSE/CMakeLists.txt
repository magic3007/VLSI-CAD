cmake_minimum_required(VERSION 3.10)
project(LSE)
file(GLOB SOURCES
        src/*.cpp
        src/*.cc
        src/*.c)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_executable(lse test.cpp)
target_link_libraries(lse ${PROJECT_NAME})
