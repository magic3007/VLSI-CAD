SET(TAUTOLOGY_BENCHMARKS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/tautology)

file(GLOB TAUTOLOGY_BENCHMARKS_LIST "${TAUTOLOGY_BENCHMARKS_DIR}/*.cube")

add_custom_target(run_tautology)
foreach (test_file IN LISTS TAUTOLOGY_BENCHMARKS_LIST)
    get_filename_component(test_name ${test_file} NAME_WE)
    add_test(
            NAME equivalence_${test_name}
            COMMAND tautology ${test_file}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    add_custom_target(run_tautology_${test_name}
            COMMAND tautology ${test_file})
    add_dependencies(run_tautology run_tautology_${test_name})
endforeach (test_file)

SET(EQUIVALENCE_BENCHMARKS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/equivalence)

MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH (child ${children})
        IF (IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        ENDIF ()
    ENDFOREACH ()
    SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(EQUIVALENCE_BENCHMARKS_DIRS_LIST ${EQUIVALENCE_BENCHMARKS_DIR})

add_custom_target(run_equivalence)

foreach (test_name ${EQUIVALENCE_BENCHMARKS_DIRS_LIST})
    add_test(
            NAME equivalence_${test_name}
            COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/equivalence_test.py "${ABC_SOLVER_BINARY}" "${EQUIVALENCE_BENCHMARKS_DIR}/${test_name}")
    add_custom_target(run_equivalence_${test_name}
            COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/equivalence_test.py "${ABC_SOLVER_BINARY}" "${EQUIVALENCE_BENCHMARKS_DIR}/${test_name}")
    add_dependencies(run_equivalence run_equivalence_${test_name})
endforeach (test_name)


SET(TECHMAP_BENCHMARKS_DIRS "\
${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/The_EPFL_Combinational_Benchmark_Suite/arithmetic;\
${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/The_EPFL_Combinational_Benchmark_Suite/random_control")

add_custom_target(run_techmap)

set(techmap_binary_path "${CMAKE_BINARY_DIR}/src/techmap")

foreach (suite_dir IN LISTS TECHMAP_BENCHMARKS_DIRS)
    get_filename_component(suite_name ${suite_dir} NAME_WE)
    file(GLOB TEST_FLIES_LIST "${suite_dir}/*.blif")
    foreach (test_file IN LISTS TEST_FLIES_LIST)
        get_filename_component(test_name ${test_file} NAME_WE)
        add_test(
                NAME techmap_${suite_name}_${test_name}
                COMMAND python3 "${CMAKE_CURRENT_SOURCE_DIR}/techmap_test.py" "${ABC_SOLVER_BINARY}" "${techmap_binary_path}" "${test_file}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
        set_tests_properties(techmap_${suite_name}_${test_name} PROPERTIES DEPENDS techmap)
        add_custom_target(run_techmap_${suite_name}_${test_name}
                COMMAND techmap ${test_file})
        add_dependencies(run_techmap run_techmap_${suite_name}_${test_name})
    endforeach (test_file)
endforeach (suite_dir)



