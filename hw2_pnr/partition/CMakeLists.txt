cmake_minimum_required(VERSION 3.10.2)

project(VLSI_CAD_LAYOUT_HW2_PARTITION)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# in case the system has multiple thread libraries 
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(partition main.cpp partition.cpp)
target_link_libraries(partition PRIVATE 
  m 
  ${CMAKE_DL_LIBS} 
  Threads::Threads)
