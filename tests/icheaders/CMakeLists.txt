set(sources
    "ic_memory.c" 
    "strings/snprintfz.c" 
    "strings/strcasecmp.c" 
    "strings/strdup.c"  
    "strings/strlwr.c" 
    "strings/strncasecmp.c" 
    "strings/strncatz.c" 
    "strings/strncpyz.c" 
    "strings/strndup.c" 
    "strings/strupr.c")

add_executable(test_icheaders
    "test.c" ${sources})
target_compile_options(test_icheaders PRIVATE -DHAVE_CONFIG_H)
set_target_properties(test_icheaders PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/icheaders"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/icheaders"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/icheaders")

add_library(test_ic ${sources})
set_target_properties(test_ic PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/icheaders"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/icheaders"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/icheaders")

enable_testing()
add_subdirectory(tests)
