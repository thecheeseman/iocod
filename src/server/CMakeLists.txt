# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(SERVER_SOURCES
    server.cpp
)
list(TRANSFORM SERVER_SOURCES PREPEND "src/")

#
# Server library
#
add_library(iocodserver SHARED ${SERVER_SOURCES})
target_link_libraries(iocodserver PRIVATE iocod_options iocod_warnings iocod_includes)

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
