# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: CC0-1.0

set(CLIENT_SOURCES
    Client.cpp
)
list(TRANSFORM CLIENT_SOURCES PREPEND "Source/")

#
# Client library
#
add_library(iocodClient SHARED ${CLIENT_SOURCES})
add_dependencies(iocodClient iocodCore)
target_link_libraries(iocodClient PRIVATE iocodOptions iocodWarnings iocodIncludes)
target_link_libraries(iocodClient PRIVATE iocodCore)

target_include_directories(iocodClient PRIVATE "Source/" "Include/")

# remove the "lib" prefix from the library name
set_target_properties(iocodClient PROPERTIES
    PREFIX ""
    ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
    LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
    RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
)

#
# Tests
#
if (BUILD_TESTS)
    add_subdirectory(Tests)
endif()
