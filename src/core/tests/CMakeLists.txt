# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: CC0-1.0

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/Catch2/extras")

enable_testing()
include(CTest)
include(Catch)

function(AddCoreTest)
    set(one_value_args NAME)
    set(multi_value_args SOURCES)
    cmake_parse_arguments(PARSE_ARGV 0 CORETEST "" "${one_value_args}" "${multi_value_args}")

    list(TRANSFORM CORETEST_SOURCES PREPEND "${CORETEST_NAME}/")

    add_executable(${CORETEST_NAME} ${CORETEST_SOURCES})
    add_dependencies(${CORETEST_NAME} iocodcore)
    target_link_libraries(${CORETEST_NAME} PRIVATE iocod_options iocod_warnings iocod_includes)
    target_link_libraries(${CORETEST_NAME} PRIVATE Catch2::Catch2WithMain iocodcore)

    set_target_properties(${CORETEST_NAME} PROPERTIES
        PREFIX ""
        ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/core-test"
        LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/core-test"
        RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/core-test"
    )
endfunction()

AddCoreTest(NAME "test-fixed_string" SOURCES "test-fixed_string.cpp")
AddCoreTest(NAME "test-narrow_cast" SOURCES "test-narrow_cast.cpp")
AddCoreTest(NAME "test-shared_library" SOURCES "test-shared_library.cpp")
AddCoreTest(NAME "test-stringed" SOURCES "test-stringed.cpp")
AddCoreTest(NAME "test-string_utilities" SOURCES "test-string_utilities.cpp")
AddCoreTest(NAME "test-vector" SOURCES "test-vector.cpp")
