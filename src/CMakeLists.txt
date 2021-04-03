
add_executable(tautology tautology_main.cpp
        sop.cpp
        sop_factory.cpp
        bdd.cpp
        bdd_factory.cpp)
target_link_libraries(tautology PRIVATE glog::glog)


add_executable(techmap techmap_main.cpp)
target_link_libraries(techmap PRIVATE glog::glog lorina)

