
add_executable(tautology tautology_main.cpp
        sop.cpp
        sop_factory.cpp
        bdd.cpp
        bdd_factory.cpp)
target_link_libraries(tautology glog::glog)


#add_executable(techmap techmap_main.cpp)
