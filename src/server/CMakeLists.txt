# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(SERVER_SOURCES
    main.cpp
    console/console.cpp
)

if (MSVC)
    list(APPEND SERVER_SOURCES
        console/console_windows.cpp
    )
else()
    list(APPEND SERVER_SOURCES
        console/console_unix.cpp
    )
endif()

list(TRANSFORM SERVER_SOURCES PREPEND "src/")

#
# Server executable
#
add_executable(iocodded ${SERVER_SOURCES})
add_dependencies(iocodded iocodcore)
target_link_libraries(iocodded PRIVATE iocod_options iocod_warnings iocod_includes iocodcore)
target_include_directories(iocodded PRIVATE "src/" "include/server/")

# remove the "lib" prefix from the library name
set_target_properties(iocodded PROPERTIES
    PREFIX ""
    ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
    LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
    RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}"
)
