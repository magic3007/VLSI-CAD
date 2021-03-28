# Download and unpack glog at configure time
configure_file(glog.cmake.in glog-download/CMakeLists.txt)
# Run CMake to configure the build tree
execute_process(COMMAND ${CMAKE_COMMAND} . -G "${CMAKE_GENERATOR}"
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/glog-download )
if(result)
    message(FATAL_ERROR "CMake step for glog failed: ${result}")
endif()
# Afterwards, generated files can be used to compile the project.
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/glog-download)
if(result)
    message(FATAL_ERROR "CMake step for glog failed: ${result}")
endif()

# Add glog directly to our build. This defines
# the glog and glog_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/glog-src
        ${CMAKE_CURRENT_BINARY_DIR}/glog-build
        EXCLUDE_FROM_ALL)

# Now simply link against glog::glog as needed. Eg
# add_executable(example example.cpp)
# target_link_libraries(example glog::glog)