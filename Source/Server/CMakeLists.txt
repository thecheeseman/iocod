# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(SERVER_SOURCES
    Main.cpp
    Console/Console.cpp
)

list(TRANSFORM SERVER_SOURCES PREPEND "Source/")

#
# Server executable
#
add_executable(iocodded ${SERVER_SOURCES})
target_link_libraries(iocodded PRIVATE iocod_options iocod_warnings iocod_includes)
target_include_directories(iocodded PRIVATE "Source/" "Include/Server/")

# remove the "lib" prefix from the library name
set_target_properties(iocodded PROPERTIES
    PREFIX ""
    ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
    LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
    RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
)
