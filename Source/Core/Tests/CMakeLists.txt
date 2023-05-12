# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/External/Catch2/extras")

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
        ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/CoreTest"
        LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/CoreTest"
        RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/CoreTest"
    )
endfunction()

AddCoreTest(NAME "Test-SharedLibrary" SOURCES "Test-SharedLibrary.cpp")
