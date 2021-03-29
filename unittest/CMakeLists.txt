SET(TAUTOLOGY_BENCHMARKS_DIR ${CMAKE_SOURCE_DIR}/benchmarks/tautology)

file(GLOB TAUTOLOGY_BENCHMARKS_LIST "${TAUTOLOGY_BENCHMARKS_DIR}/*.cube")

foreach (test_file IN LISTS TAUTOLOGY_BENCHMARKS_LIST)
    get_filename_component(test_name ${test_file} NAME_WE)
    add_test(
            NAME ${test_name}
            COMMAND tautology ${test_file}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    add_custom_target(run_${test_name}
            COMMAND tautology ${test_file})
endforeach (test_file)