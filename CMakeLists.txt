# iocod
# Copyright (C) 2021-2022 thecheeseman
# 
# This file is part of the iocod GPL source code.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.13)

project("iocod"
    VERSION 0.30.0
    DESCRIPTION "open source CoD1.1-1.5 project"
    HOMEPAGE_URL "https://iocod.org"
    LANGUAGES C)

# at a minimum, we need C11
# this is supported across the board already on 
# GCC/GNUC systems since ~2013 in GCC 4.6, Clang 3.1
# and on Windows since ~2019 in VS2019 16.8 (MSVC 14.28)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS TRUE)

# enable custom modules
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")

#
# set default built type / paths
#
message(STATUS "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "CMAKE_BUILD_TYPE not specified, default is 'Debug'")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
else()
  message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(DEBUG 1)
else()
  set(DEBUG 0)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
foreach(CFGNAME ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${CFGNAME} CFGNAME)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CFGNAME} ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CFGNAME} ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CFGNAME} ${CMAKE_BINARY_DIR}/lib)
endforeach()

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
  STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

#
# architecture checks
#
include(CheckArchitecture)

#
# main options
#

# if we're building on a 64-bit system, allow the user to optionally
# force build a 32-bit version
if (IC_64BIT)
    set(IC_CMAKE_64BIT TRUE)
    option(BUILD_32_BIT "Build 32-bit binaries" FALSE)

    if (BUILD_32_BIT)
        set(IC_CMAKE_64BIT FALSE)

        if (IC_PLATFORM STREQUAL "amd64")
            set(IC_PLATFORM "i386")
        endif()

        if (IC_PLATFORM STREQUAL "arm64")
            set(IC_PLATFORM "arm32")
        endif()
    endif()
endif()

option(BUILD_PRELOAD "Build preload library" OFF)
option(BUILD_PROTOTYPEGEN "Build script prototype generator" OFF) 
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_RUNTIME_ZIP "Build .zip of runtime files" ON)

#
# check for system/compiler properties and header files
#
include(CheckCSourceCompiles)
include(CheckIncludeFiles)
include(CheckSymbolExists)

include(CheckStringHeaders)
check_string_headers()

if (NOT WIN32)
    check_include_files(fcntl.h HAVE_FCNTL_H)
endif()

#
# config.h
#
configure_file("config.h.in" "config.h")
include_directories(${PROJECT_BINARY_DIR})
add_compile_options(-DHAVE_CONFIG_H)

#
# headers
#
#include_directories("${CMAKE_SOURCE_DIR}/include")          # API headers
include_directories("src/external")     # TODO: this needs to be moved to /external/
include_directories("src/include")      # internal headers
include_directories("src")

if (WIN32)
    include_directories("external/dlfcn-win32")
    include_directories("external/dirent")
endif()

include_directories("external/miniz")

#
# link directory
#
link_directories("${CMAKE_SOURCE_DIR}/lib")

#
# compile options
#
# build options from q3
# -O3 -ffast-math -fomit-frame-pointer -fno-strict-aliasing
if (MSVC)
    # silence some warnings
    add_compile_options(/wd4244) # conversion from 'double' to 'float'
    add_compile_options(/wd4305) # truncation from 'double' to 'float'
    add_compile_options(/wd4996) # unsafe functions strcpy etc

    # winbase.h(9531): warning C5105: 
    # macro expansion producing 'defined' has undefined behavior
    add_compile_options(/wd5105)
else()
    add_compile_options(-Wall -Wextra -Wformat=2)

    if (NOT APPLE)
        add_compile_options(-Wformat-overflow=2)
    endif()
endif()

#
# TARGETS
#

# 1. iocod -> main server exe
if (WIN32)
    add_executable(iocod WIN32 "")
else()
    add_executable(iocod "")
endif()

# 2. iocod_game -> game code
add_library(iocod_game SHARED "")

#
# main source directory
#
add_subdirectory("src")

#
# external libraries
#
add_subdirectory("external")

#
# various options
#
if (BUILD_PRELOAD AND UNIX AND NOT (IC_64BIT AND BUILD_32_BIT))
    add_subdirectory("preload") # preload library if we're on unix/32bit only
endif()

if (BUILD_PROTOTYPEGEN)
    add_subdirectory("prototypegen") # prototype generators for scripts
endif()

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory("tests") # test suites
endif()

#
#
#
if (IC_64BIT)
    set(PLAT_BITS "64")
else()
    set(PLAT_BITS "32")
endif()

# if we want to build 32-bit binaries
if (IC_64BIT AND BUILD_32_BIT)
    set(32_BIT_OPTS "")
    set(PLAT_BITS "32")
    
    if (WIN32)
        set(32_BIT_OPTS "-DWIN32")
    else()
        set(32_BIT_OPTS "-m32")
    endif()

    target_compile_options(iocod PRIVATE "${32_BIT_OPTS}")
    target_link_options(iocod PRIVATE "${32_BIT_OPTS}")
    target_compile_options(iocod_game PRIVATE "${32_BIT_OPTS}")
    target_link_options(iocod_game PRIVATE "${32_BIT_OPTS}")
endif()

set(PLAT_EXE "")
set(PLAT_DLL "")
if (WIN32)
    set(PLAT_EXE ".exe")
    set(PLAT_DLL ".dll")
else()
    if (APPLE)
        set(PLAT_DLL ".dylib")
    else()
        set(PLAT_DLL ".so")
    endif()
endif()

set_target_properties(iocod PROPERTIES OUTPUT_NAME "iocod${PLAT_BITS}")

#
# PREFIX disable "lib" prefix on Unix systems
# LIBRARY_OUTPUT_DIRECTORY overrides the settings we set at the top
# so that the static libraries get put in lib/ and this dll gets put
# with the binaries
#
set_target_properties(iocod_game PROPERTIES
    PREFIX ""
    OUTPUT_NAME "icgame${PLAT_BITS}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/main"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/main")

#
# linker options
#
if (WIN32)
    target_link_libraries(iocod dl getopt winmm miniz curl)
    target_link_libraries(iocod_game)
else()
    target_link_libraries(iocod ${CMAKE_DL_LIBS} m miniz curl)
    target_link_libraries(iocod_game m)
endif()

# 
# zip
# 
if (BUILD_RUNTIME_ZIP)
    set(TAR_ARGS "cvfz")
    set(TAR_EXT ".tgz")
    set(TAR_FMT "")

    if (WIN32)
        set(TAR_ARGS "cvf")
        set(TAR_EXT ".zip")
        set(TAR_FMT --format=zip)
    endif()

    set(TAR_OUT "../iocod-${iocod_VERSION}-${IC_OS}-${IC_PLATFORM}${TAR_EXT}")

    add_custom_target(create_zip ALL DEPENDS
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
        COMMAND ${CMAKE_COMMAND} -E tar "${TAR_ARGS}" "${TAR_OUT}"
        ${TAR_FMT} --
        "iocod${PLAT_BITS}${PLAT_EXE}"
        "main/icgame${PLAT_BITS}${PLAT_DLL}"
        "${CMAKE_SOURCE_DIR}/README.md"
    )
    add_dependencies(create_zip iocod iocod_game)
endif()
