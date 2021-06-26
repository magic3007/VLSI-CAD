cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "SequencePair")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES src/*.h src/ConstraintGraph.cc)
file(GLOB EXESRCS src/main.cc)

add_executable(${PROJECT_NAME} ${SOURCES} ${EXESRCS})
