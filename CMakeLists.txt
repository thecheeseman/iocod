# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
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

if (BUILD_32BIT)
    add_compile_options(-m32 -fPIC) # TODO: optionally enable this for just hook builds
    add_link_options(-m32 -fPIC)    # TODO: optionally enable this for just hook builds
endif()

add_library(iocod_options INTERFACE)
EnableInterproceduralOptimization(iocod_options)
EnableSanitizers(iocod_options)

if (ENABLE_UNITY_BUILD)
    set_target_properties(iocod_options PROPERTIES UNITY_BUILD ON)
endif()

target_compile_definitions(iocod_options INTERFACE
    $<$<CONFIG:Debug>:IOCOD_DEBUG>
)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_options(iocod_options INTERFACE
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
    )

    target_link_options(iocod_options INTERFACE
        -fno-omit-frame-pointer
        -ffast-math
        -fvisibility=hidden
        -fvisibility-inlines-hidden
    )

    target_compile_definitions(iocod_options INTERFACE
        -D_GLIBCXX_ASSERTIONS
        -D_FORTIFY_SOURCE=3
    )
elseif (MSVC)
    target_compile_options(iocod_options INTERFACE
        $<$<CONFIG:Debug>:/Od /Zi>
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:RelWithDebInfo>:/O2 /Zi>
        $<$<CONFIG:MinSizeRel>:/Os>

        /utf-8                                  # use UTF-8 for source and execution

        /sdl                                    # enable additional security checks
        /guard:cf                               # enable control flow guard

        /Zc:__cplusplus                         # report the correct C++ standard
        /Zc:__STDC__                            # report the correct C standard
        /Zc:char8_t                             # enable char8_t, the new C++20 char type
    )
    target_link_options(iocod_options INTERFACE
        $<$<CONFIG:Debug>:/DEBUG:FASTLINK>

        /DYNAMICBASE                            # enable ASLR
        /CETCOMPAT                              # enable CET
        /NXCOMPAT                               # enable DEP
    )
endif()

#
# warnings
#
add_library(iocod_warnings INTERFACE)
SetProjectWarnings(iocod_warnings)

#
# staged install
#
set(STAGED_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/staged")
message(STATUS "Staged install prefix: ${STAGED_INSTALL_PREFIX}")

#
# includes
#
add_library(iocod_includes INTERFACE)
target_include_directories(iocod_includes INTERFACE
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/src/core/include"
    "${CMAKE_SOURCE_DIR}/src/game/include"
    "${STAGED_INSTALL_PREFIX}/include"

    # external
    "${CMAKE_SOURCE_DIR}/external/expected-lite/include"
    "${CMAKE_SOURCE_DIR}/external/fmt/include"
    "${CMAKE_SOURCE_DIR}/external/spdlog/include"
)

#
# projects
#
include(iocodGetGitCommitInfo)

add_subdirectory(external)
add_subdirectory(src)
