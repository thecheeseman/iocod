add_executable(test_fs "fs.c" "fs_zip.c" "fs_hash_filename.c")
target_link_libraries(test_fs test_ic miniz)
set_target_properties(test_fs PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/fsnew"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/fsnew"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/fsnew")
