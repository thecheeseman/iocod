# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: CC0-1.0

set(SERVER_SOURCES
    server.cpp
)
list(TRANSFORM SERVER_SOURCES PREPEND "src/")

#
# Server library
#
add_library(iocodserver SHARED ${SERVER_SOURCES})
add_dependencies(iocodserver iocodcore)
target_link_libraries(iocodserver PRIVATE iocod_options iocod_warnings iocod_includes)
target_link_libraries(iocodserver PRIVATE iocodcore)

target_include_directories(iocodserver PRIVATE "src/" "include/")

# remove the "lib" prefix from the library name
set_target_properties(iocodserver PROPERTIES
    PREFIX ""
    ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
    LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
    RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
)

#
# Tests
#
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
