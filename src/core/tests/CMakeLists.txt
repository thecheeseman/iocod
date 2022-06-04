project(core_tests C)

include(CTest)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -C ${CMAKE_CFG_INTDIR})
function(add_core_test TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})

    add_test(NAME ${TEST_NAME} 
        WORKING_DIRECTORY $<TARGET_FILE_DIR:${TEST_NAME}>
        COMMAND $<TARGET_FILE:${TEST_NAME}>)
    add_dependencies(check ${TEST_NAME})
    target_link_libraries(${TEST_NAME} $<TARGET_OBJECTS:confuse> iccore)

    if (WIN32)
        target_link_options(${TEST_NAME} BEFORE PUBLIC "/SUBSYSTEM:CONSOLE,5.01")
    endif()

    if ((${TEST_NAME} STREQUAL "test-network" OR ${TEST_NAME} STREQUAL "test-netadr") AND WIN32)
        target_link_libraries(${TEST_NAME} Ws2_32)
    endif()

    set_target_properties(${TEST_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/${TEST_NAME}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/${TEST_NAME}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/${TEST_NAME}")

    message(STATUS "Added iccore test '${TEST_NAME}' (${ARGN})")
endfunction()

file(GLOB TEST_SRCS "*.c")
foreach (test IN LISTS TEST_SRCS)
    # strip file path to get clean test name
    get_filename_component(TEST_NAME ${test} NAME)
    string(REPLACE ".c" "" TEST_NAME ${TEST_NAME})

    add_core_test(${TEST_NAME} ${test})
endforeach()
