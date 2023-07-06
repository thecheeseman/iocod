# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: CC0-1.0

enable_testing()
include(CTest)

function(AddEngineTest)
    set(one_value_args NAME)
    set(multi_value_args SOURCES)
    cmake_parse_arguments(PARSE_ARGV 0 ENGINETEST "" "${one_value_args}" "${multi_value_args}")

    add_executable(${ENGINETEST_NAME} ${ENGINETEST_NAME}.cpp ${ENGINETEST_SOURCES})
    add_dependencies(${ENGINETEST_NAME} iocodEngine)
    target_link_libraries(${ENGINETEST_NAME} PRIVATE iocodOptions iocodWarnings iocodIncludes)
    target_link_libraries(${ENGINETEST_NAME} PRIVATE doctest iocodEngine)

    set_target_properties(${ENGINETEST_NAME} PROPERTIES
        PREFIX ""
        ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/EngineTests"
        LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/EngineTests"
        RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/EngineTests"
    )
endfunction()

AddEngineTest(NAME "Test-ByteSwap")
AddEngineTest(NAME "Test-Color")
AddEngineTest(NAME "Test-Span")
#AddEngineTest(NAME "Test-String")
