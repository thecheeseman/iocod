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

    add_executable(${CORETEST_NAME} ${CORETEST_NAME}.cpp ${CORETEST_SOURCES})
    add_dependencies(${CORETEST_NAME} iocodEngine)
    target_link_libraries(${CORETEST_NAME} PRIVATE iocodOptions iocodWarnings iocodIncludes)
    target_link_libraries(${CORETEST_NAME} PRIVATE Catch2::Catch2WithMain iocodEngine)

    set_target_properties(${CORETEST_NAME} PROPERTIES
        PREFIX ""
        ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/core-test"
        LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/core-test"
        RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/core-test"
    )
endfunction()

AddCoreTest(NAME "test-array")
AddCoreTest(NAME "test-fixed_string")
AddCoreTest(NAME "test-narrow_cast")
AddCoreTest(NAME "test-shared_library")
AddCoreTest(NAME "Test-String")
#AddCoreTest(NAME "test-stringed")
#AddCoreTest(NAME "test-string_utilities")
AddCoreTest(NAME "test-timer")
AddCoreTest(NAME "test-vector")
AddCoreTest(NAME "test-version")
