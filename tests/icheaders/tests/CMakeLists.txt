include (CTest)
add_custom_target(check_iccore COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -C ${CMAKE_CFG_INTDIR})
function(add_icheaders_test TEST_NAME)
    add_executable(${TEST_NAME} EXCLUDE_FROM_ALL ${ARGN})
    add_test(NAME ${TEST_NAME} 
        WORKING_DIRECTORY $<TARGET_FILE_DIR:${TEST_NAME}>
        COMMAND $<TARGET_FILE:${TEST_NAME}>)
    add_dependencies(check ${TEST_NAME})
    target_link_libraries(${TEST_NAME} test_ic)

    set_target_properties(${TEST_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/${TEST_NAME}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/${TEST_NAME}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/${TEST_NAME}")
endfunction()

add_icheaders_test(test-snprintfz   test-snprintfz.c)
add_icheaders_test(test-strcasecmp  test-strcasecmp.c)
add_icheaders_test(test-strdup      test-strdup.c)
add_icheaders_test(test-strlwr      test-strlwr.c)
add_icheaders_test(test-strncasecmp test-strncasecmp.c)
add_icheaders_test(test-strncatz    test-strncatz.c)
add_icheaders_test(test-strncpyz    test-strncpyz.c)
add_icheaders_test(test-strndup     test-strndup.c)
add_icheaders_test(test-strupr      test-strupr.c)
