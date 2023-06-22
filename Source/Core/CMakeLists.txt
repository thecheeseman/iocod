# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: CC0-1.0

set(CORE_SOURCES
    Assert.cpp
    CommandSystem.cpp
    #cvar_system.cpp
    FilePath.cpp
    FileSystem.cpp
    Memory.cpp
    Plugin.cpp
    ScopedTimer.cpp
    String.cpp
    StringEd.cpp
    StringUtilities.cpp
    System/SharedLibrary.cpp
    System/System.cpp
    System/SystemInfo.cpp
    Vector.cpp
)

if (MSVC)
    list(APPEND CORE_SOURCES
        System/ConsoleWin32.cpp
    )
else()
    list(APPEND CORE_SOURCES
        System/ConsoleUnix.cpp
    )
endif()

list(TRANSFORM CORE_SOURCES PREPEND "Source/")

#
# Core library
#
add_library(iocodCore STATIC ${CORE_SOURCES})
target_compile_definitions(iocodCore PRIVATE IOCOD_BUILD_DLL)
target_link_libraries(iocodCore PRIVATE iocodOptions iocodWarnings iocodIncludes)
target_link_libraries(iocodCore PRIVATE fmt::fmt spdlog::spdlog miniz)

# remove the "lib" prefix from the library name
set_target_properties(iocodCore PROPERTIES
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
