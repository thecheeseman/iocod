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
    VERSION 0.103.0
    DESCRIPTION "open source CoD1.1-1.5 project"
    HOMEPAGE_URL "https://iocod.org"
    LANGUAGES C)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(SEND_ERROR "In-source builds are not allowed")
endif()

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
option(BUILD_CORE "Build core library" ON)
option(BUILD_MAIN "Build main executables and game dll" OFF)
option(BUILD_EXTERNAL "Build external libraries" OFF)
option(BUILD_TESTS "Build tests" OFF)

option(BUILD_PRELOAD "Build preload library" OFF)
option(BUILD_PROTOTYPEGEN "Build script prototype generator" OFF) 
option(BUILD_RUNTIME_ZIP "Build .zip of runtime files" OFF)

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
configure_file("config.h.in" "${PROJECT_BINARY_DIR}/config.h")
include_directories(${PROJECT_BINARY_DIR})
add_compile_options(-DHAVE_CONFIG_H)

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
enable_testing()

#
# core library
# 
if (BUILD_CORE)
    add_subdirectory("src/core")
endif()

#
# main source directory
#
if (BUILD_MAIN)
    add_subdirectory("src")
endif()

#
# external libraries
#
if (BUILD_EXTERNAL)
    add_subdirectory("external")
endif()

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
    message(STATUS "hello")
    enable_testing()
    add_subdirectory("tests" EXCLUDE_FROM_ALL) # test suites
endif()

# 
# zip
# 
if (BUILD_RUNTIME_ZIP)
    set(TAR_ARGS "cvfz")
    set(TAR_EXT ".tar.gz")
    set(TAR_FMT "")
    set(TAR_OS "${iocod_OS}${IC_PLATFORM_BITS}")

    if (WIN32)
        set(TAR_ARGS "cvf")
        set(TAR_EXT ".zip")
        set(TAR_FMT --format=zip)
    endif()

    set(TAR_OUT "../iocod-${iocod_VERSION}-${TAR_OS}${TAR_EXT}")

    add_custom_target(create_zip ALL DEPENDS
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
        COMMAND ${CMAKE_COMMAND} -E tar "${TAR_ARGS}" "${TAR_OUT}"
        ${TAR_FMT} --
        "iocod${IC_PLATFORM_BITS}${PLAT_EXE}"
        "main/icgame${IC_PLATFORM_BITS}${PLAT_DLL}"
        "${CMAKE_SOURCE_DIR}/README.md"
    )
    add_dependencies(create_zip iocod iocod_game)
endif()
