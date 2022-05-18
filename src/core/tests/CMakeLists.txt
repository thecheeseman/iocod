project(core_tests)

include (CTest)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -C ${CMAKE_CFG_INTDIR})
function(add_core_test TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})
    add_test(NAME ${TEST_NAME} 
        WORKING_DIRECTORY $<TARGET_FILE_DIR:${TEST_NAME}>
        COMMAND $<TARGET_FILE:${TEST_NAME}>)
    add_dependencies(check ${TEST_NAME})
    target_link_libraries(${TEST_NAME} iccore)

    set_target_properties(${TEST_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/${TEST_NAME}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/${TEST_NAME}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/${TEST_NAME}")
endfunction()

add_core_test(test-cvar test-cvar.c)
add_core_test(test-logger test-logger.c)
add_core_test(test-default-ext test-default-ext.c)
add_core_test(test-infostring test-infostring.c)
add_core_test(test-parse test-parse.c)
add_core_test(test-va test-va.c)
