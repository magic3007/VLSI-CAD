include(googletest.cmake)
include(glog.cmake)

add_subdirectory(abc)
set(ABC_SOLVER_BINARY ${CMAKE_CURRENT_BINARY_DIR}/abc/abc PARENT_SCOPE)