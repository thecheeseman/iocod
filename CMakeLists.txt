# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: CC0-1.0

cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")
project(iocod LANGUAGES C CXX)

# disallow in-source builds
include(AssureOutOfSourceBuilds)
AssureOutOfSourceBuilds()

# includes
include(CheckBuildType)
include(EnableCompileCommands)
include(EnableCompilerDiagnostics)
include(EnableSanitizers)
include(Optimization)
include(SetProjectWarnings)
include(SetStandards)
#

#
# options
#
include(iocodOptions)

add_library(iocodOptions INTERFACE)
EnableInterproceduralOptimization(iocodOptions)
EnableSanitizers(iocodOptions)

if (ENABLE_UNITY_BUILD)
    set_target_properties(iocodOptions PROPERTIES UNITY_BUILD ON)
endif()

target_compile_definitions(iocodOptions INTERFACE
    $<$<CONFIG:Debug>:IOCOD_DEBUG>
)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_options(iocodOptions INTERFACE
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O2>
        $<$<CONFIG:RelWithDebInfo>:-g -O2>
        $<$<CONFIG:MinSizeRel>:-Os>
        $<$<CXX_COMPILER_ID:Clang>:-fno-inline> # Clang over-inlines some important things
                                                # causing segfaults... TODO: fix this
        -fno-omit-frame-pointer
        -ffast-math
        -fvisibility=hidden
        -fvisibility-inlines-hidden
        -fno-rtti
    )

    target_link_options(iocodOptions INTERFACE
        -fno-omit-frame-pointer
        -ffast-math
        -fvisibility=hidden
        -fvisibility-inlines-hidden
    )

    target_compile_definitions(iocodOptions INTERFACE
        -D_GLIBCXX_ASSERTIONS
        -D_FORTIFY_SOURCE=3
    )
elseif (MSVC)
    target_compile_options(iocodOptions INTERFACE
        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:RelWithDebInfo>:/O2 /Zi>
        $<$<CONFIG:MinSizeRel>:/Os>

        /utf-8                                  # use UTF-8 for source and execution

        /EHsc                                   # enable C++ exceptions (ISO C++ EH)

        /sdl                                    # enable additional security checks
        /guard:cf                               # enable control flow guard

        /Zc:__cplusplus                         # report the correct C++ standard
        /Zc:__STDC__                            # report the correct C standard
        /Zc:char8_t                             # enable char8_t, the new C++20 char type
    )

    target_link_options(iocodOptions INTERFACE
        $<$<CONFIG:Debug>:/DEBUG:FASTLINK>

        /DYNAMICBASE                            # enable ASLR
        /CETCOMPAT                              # enable CET
        /NXCOMPAT                               # enable DEP
    )

    target_compile_options(iocodOptions INTERFACE
        -D_CRT_SECURE_NO_WARNINGS
    )
endif()

#
# warnings
#
add_library(iocodWarnings INTERFACE)
SetProjectWarnings(iocodWarnings)

#
# includes
#
add_library(iocodIncludes INTERFACE)
target_include_directories(iocodIncludes INTERFACE
    "${CMAKE_SOURCE_DIR}/Include"
    "${CMAKE_SOURCE_DIR}/Source/Engine/Include"
    "${CMAKE_SOURCE_DIR}/Source/Game/Include"
    "${STAGED_INSTALL_PREFIX}/Include"

    # external
    "${CMAKE_SOURCE_DIR}/External/expected-lite/include"
    "${CMAKE_SOURCE_DIR}/External/fmt/include"
    "${CMAKE_SOURCE_DIR}/External/miniz"
    "${CMAKE_SOURCE_DIR}/External/spdlog/include"
    "${CMAKE_SOURCE_DIR}/External/nanobench/src/include"
    "${CMAKE_SOURCE_DIR}/External/doctest"
)
target_precompile_headers(iocodIncludes INTERFACE
    <cstddef>
    <cstdint>
    <type_traits>

    <EngineDefines.h>
    <EngineTypes.h>
    <Version.h>

    <Core/Memory.h>
)

#
# staged install
#
set(STAGED_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/Staged")
message(STATUS "Staged install prefix: ${STAGED_INSTALL_PREFIX}")

#
# projects
#
include(iocodGetGitCommitInfo)

# probably shouldn't have this here but it's convenient for everything to have fmt
add_subdirectory(External)
target_link_libraries(iocodIncludes INTERFACE fmt::fmt)

add_subdirectory(Source)
