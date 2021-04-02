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