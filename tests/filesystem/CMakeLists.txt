# local files
file(GLOB FS_SOURCES "*.c")

#foreach (subdir
#    ../../src/strings)
#    file(GLOB sources "${subdir}/*.c")
#    list(APPEND FS_SOURCES ${sources})
#endforeach()

add_executable(test_filesystem "${FS_SOURCES}")

if (WIN32)
    target_compile_options(test_filesystem PUBLIC /FI no_com.h)
else()
    target_compile_options(test_filesystem PUBLIC -include no_com.h)
endif()

target_compile_options(test_filesystem PUBLIC -DFS_STANDALONE)
target_link_libraries(test_filesystem ic_cvar ic_string ic_infostring)
set_target_properties(test_filesystem PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/filesystem"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/filesystem"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/filesystem")
