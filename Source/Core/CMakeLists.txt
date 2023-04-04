# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(CORE_SOURCES
    Assert.cpp
    Version.cpp
)
list(TRANSFORM CORE_SOURCES PREPEND "Source/")

#
# Core library
#
add_library(iocodcore STATIC ${CORE_SOURCES})
target_link_libraries(iocodcore PRIVATE PRIVATE iocod_options iocod_warnings iocod_includes)

# remove the "lib" prefix from the library name
set_target_properties(iocodcore PROPERTIES
    PREFIX ""
    ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
    LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
    RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
)