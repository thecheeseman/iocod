# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: CC0-1.0

set(ENGINE_SOURCES
    Application.cpp
    Assert.cpp
    Engine.cpp
    Log.cpp
    Memory.cpp
    Platform/Platform.cpp
    Platform/SharedLibrary.cpp
    ScopedTimer.cpp
    String.cpp
)

if (WIN32)
    list(APPEND ENGINE_SOURCES
        Platform/Windows/PlatformWindows.cpp
    )
elseif(APPLE)
    list(APPEND ENGINE_SOURCES
        Platform/MacOS/PlatformMacOS.mm
    )
else()
    list(APPEND ENGINE_SOURCES
        Platform/Linux/PlatformLinux.cpp
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
