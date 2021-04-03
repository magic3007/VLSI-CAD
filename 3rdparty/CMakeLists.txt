include(googletest.cmake)
include(glog.cmake)

add_subdirectory(abc)

set(ABC_SOLVER_BINARY "${CMAKE_CURRENT_BINARY_DIR}/abc/abc" PARENT_SCOPE)

add_library(abc_solver::static_lib STATIC IMPORTED GLOBAL)
set_target_properties(abc_solver::static_lib PROPERTIES
        INTERFACE_LINK_LIBRARIES ${CMAKE_DL_LIBS}
        IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/abc/libabc.a"
        INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/abc/src")

add_subdirectory(lorina)