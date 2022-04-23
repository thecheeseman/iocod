add_executable(test-snprintfz test-snprintfz.c)
target_link_libraries(test-snprintfz test_ic)
set_target_properties(test-snprintfz PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/icheaders/snprintfz"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/icheaders/snprintfz"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/icheaders/snprintfz")
add_test(NAME test-snprintfz COMMAND test-snprintfz 
    WORKING_DIRECTORY $<TARGET_FILE_DIR:test-snprintfz>)
