# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: CC0-1.0

set(ENGINE_SOURCES
    Allocator.cpp
    Application.cpp
    Assert.cpp
    Engine.cpp
    Log.cpp
    Memory.cpp
    Platform/SharedLibrary.cpp
    ScopedTimer.cpp
    #String.cpp

    eastl_iocod.cpp
)

if (WIN32)
    list(APPEND ENGINE_SOURCES
        Platform/Windows/ConsoleWindows.cpp
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
target_link_libraries(iocodEngine PRIVATE iocodOptions iocodWarnings iocodIncludes miniz rpmalloc)
target_link_libraries(iocodEngine PUBLIC fmt::fmt spdlog::spdlog EASTL EAStdC) #  EAAssert EAThread

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

    add_custom_command(
        TARGET iocodEngine
        POST_BUILD
        COMMENT "Copy iocodEngine to test folder"
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:iocodEngine>
            "${STAGED_INSTALL_PREFIX}/EngineTests"
    )
endif()

add_custom_command(
    TARGET iocodEngine
    POST_BUILD
    COMMENT "Generate sha512 checksum for iocodEngine"
    WORKING_DIRECTORY $<TARGET_FILE_DIR:iocodEngine>
    COMMAND ${CMAKE_COMMAND} -E sha512sum $<TARGET_FILE_NAME:iocodEngine> > $<TARGET_FILE_NAME:iocodEngine>.sha512
    VERBATIM
)
