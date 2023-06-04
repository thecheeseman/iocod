# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(CLIENT_SOURCES
    client.cpp
)
list(TRANSFORM CLIENT_SOURCES PREPEND "src/")

#
# Client library
#
add_library(iocodclient SHARED ${CLIENT_SOURCES})
target_link_libraries(iocodclient PRIVATE iocod_options iocod_warnings iocod_includes)

target_include_directories(iocodclient PRIVATE "src/" "include/")

# remove the "lib" prefix from the library name
set_target_properties(iocodclient PROPERTIES
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
