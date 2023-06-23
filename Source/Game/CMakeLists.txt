# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: CC0-1.0

set(GAME_SOURCES
    Game.cpp
)
list(TRANSFORM GAME_SOURCES PREPEND "Source/")

#
# Game library
#
add_library(iocodGame SHARED ${GAME_SOURCES})
add_dependencies(iocodGame iocodCore)
target_link_libraries(iocodGame PRIVATE iocodOptions iocodWarnings iocodIncludes)
target_link_libraries(iocodGame PRIVATE iocodCore)

target_include_directories(iocodGame PRIVATE "Source/" "Include/")

# remove the "lib" prefix from the library name
set_target_properties(iocodGame PROPERTIES
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
