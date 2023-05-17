# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

if (NOT BUILD_32BIT)
    message(STATUS "Skipping 32-bit hook build (BUILD_32BIT=OFF)")
    return()
endif()

set(HOOK_SOURCES
    LDPreload.cpp
    Main.cpp
)
list(TRANSFORM HOOK_SOURCES PREPEND "Source/")

#
# LD_PRELOAD hook
#
add_library(iocodhook SHARED ${HOOK_SOURCES})
target_link_libraries(iocodhook PRIVATE iocod_options iocod_warnings iocod_includes)

# remove the "lib" prefix from the library name
set_target_properties(iocodhook PROPERTIES
    PREFIX ""
    ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
    LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
    RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
)