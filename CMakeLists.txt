# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

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
add_compile_options(-m32 -fPIC) # TODO: optionally enable this for just hook builds
add_link_options(-m32 -fPIC)    # TODO: optionally enable this for just hook builds

include(iocodOptions)
add_library(iocod_options INTERFACE)
EnableInterproceduralOptimization(iocod_options)
EnableSanitizers(iocod_options)

if (ENABLE_UNITY_BUILD)
    set_target_properties(iocod_options PROPERTIES UNITY_BUILD ON)
endif()

target_link_options(iocod_options INTERFACE
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

target_compile_options(iocod_options INTERFACE
    -fno-omit-frame-pointer
    -ffast-math
    -fvisibility=hidden
    -fvisibility-inlines-hidden
)

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
    "${CMAKE_SOURCE_DIR}/Include"
    "${CMAKE_SOURCE_DIR}/Source/Core/Include"
    "${STAGED_INSTALL_PREFIX}"
)

#
# projects
#
add_subdirectory("Source/Core")
add_subdirectory("Source/Hook")
