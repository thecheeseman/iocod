# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: CC0-1.0

set(ENGINE_SOURCES
    Assert.cpp
    #CommandSystem.cpp
    #cvar_system.cpp
    #FilePath.cpp
    #FileSystem.cpp
    Log.cpp
    Memory.cpp
    #Plugin.cpp
    ScopedTimer.cpp
    String.cpp
    #StringEd.cpp
    #StringUtilities.cpp
    System/SharedLibrary.cpp
    #System/System.cpp
    #System/SystemInfo.cpp
    #Vector.cpp
)

if (MSVC)
    list(APPEND ENGINE_SOURCES
        #System/ConsoleWin32.cpp
    )
else()
    list(APPEND ENGINE_SOURCES
        #System/ConsoleUnix.cpp
    )
endif()

list(TRANSFORM ENGINE_SOURCES PREPEND "Source/")

#
# Engine library
#
add_library(iocodEngine SHARED ${ENGINE_SOURCES})
target_compile_definitions(iocodEngine PUBLIC -DIOCOD_BUILD_ENGINE)
target_link_libraries(iocodEngine PRIVATE iocodOptions iocodWarnings iocodIncludes)
target_link_libraries(iocodEngine PRIVATE fmt::fmt spdlog::spdlog miniz)

# remove the "lib" prefix from the library name
set_target_properties(iocodEngine PROPERTIES
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
