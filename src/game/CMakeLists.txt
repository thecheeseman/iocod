# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(GAME_SOURCES
    main.cpp
)
list(TRANSFORM GAME_SOURCES PREPEND "src/")

#
# Game library
#
add_library(iocodgame SHARED ${GAME_SOURCES})
target_link_libraries(iocodgame PRIVATE iocod_options iocod_warnings iocod_includes)

# remove the "lib" prefix from the library name
set_target_properties(iocodgame PROPERTIES
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
