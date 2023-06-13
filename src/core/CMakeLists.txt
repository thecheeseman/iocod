# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: CC0-1.0

set(CORE_SOURCES
    assert.cpp
    command_system.cpp
    #cvar_system.cpp
    file_path.cpp
    file_system.cpp
    plugin.cpp
    scoped_timer.cpp
    string.cpp
    stringed.cpp
    string_utilities.cpp
    system/shared_library.cpp
    system/system.cpp
    system/system_info.cpp
    vector.cpp
)

if (MSVC)
    list(APPEND CORE_SOURCES
        system/console_win32.cpp
    )
else()
    list(APPEND CORE_SOURCES
        system/console_unix.cpp
    )
endif()

list(TRANSFORM CORE_SOURCES PREPEND "src/")

#
# Core library
#
add_library(iocodcore STATIC ${CORE_SOURCES})
target_compile_definitions(iocodcore PRIVATE IOCOD_BUILD_DLL)
target_link_libraries(iocodcore PRIVATE iocod_options iocod_warnings iocod_includes)
target_link_libraries(iocodcore PRIVATE fmt::fmt spdlog::spdlog miniz)

# remove the "lib" prefix from the library name
set_target_properties(iocodcore PROPERTIES
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
