add_executable(test-dlfcn "test-dlfcn.c")
set_target_properties(test-dlfcn PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/dlfcn"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/dlfcn"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/dlfcn")
target_link_libraries(test-dlfcn dl)
