# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: CC0-1.0

set(SERVER_SOURCES
    Server.cpp
)
list(TRANSFORM SERVER_SOURCES PREPEND "Source/")

#
# Server library
#
add_library(iocodServer SHARED ${SERVER_SOURCES})
add_dependencies(iocodServer iocodEngine)
target_link_libraries(iocodServer PRIVATE iocodOptions iocodWarnings iocodIncludes)
target_link_libraries(iocodServer PRIVATE iocodEngine)

target_include_directories(iocodServer PRIVATE "Source/" "Include/")

# remove the "lib" prefix from the library name
set_target_properties(iocodServer PROPERTIES
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
