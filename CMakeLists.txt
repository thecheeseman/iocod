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
project("iocod" C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS TRUE)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/cmake")

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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
foreach(CFGNAME ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${CFGNAME} CFGNAME)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CFGNAME} ${CMAKE_BINARY_DIR}/out)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CFGNAME} ${CMAKE_BINARY_DIR}/out)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CFGNAME} ${CMAKE_BINARY_DIR}/out)
endforeach()

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
  STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

#
# main options
#
option(COMPILE_32_BIT "Compile 32-bit binaries and game dll" FALSE)
option(USE_CURL "Use cURL library" FALSE)
option(DISABLE_AUTO_UPDATE "Disable auto-update feature" FALSE)
option(USE_LUA "Use Lua as scripting language instead of GSC" FALSE)
option(USE_SQLITE3 "Use SQLite3" FALSE)

#
# testing-only options
#
if (DEBUG)
    option(BUILD_PRELOAD "Build preload library" OFF)
    option(BUILD_PROTOTYPEGEN "Build script prototype generator" OFF) 
    option(BUILD_TESTS "Build tests" ON)
endif()

#
# check for system/compiler properties and header files
#
include(CheckCSourceCompiles)
include(CheckIncludeFiles)
include(CheckSymbolExists)

include(Check64Bit)
check_64_bit()

include(CheckStringHeaders)
check_string_headers()

if (NOT WIN32)
    check_include_files(fcntl.h HAVE_FCNTL_H)
endif()

#
# check for zlib
#
include(CheckAndGrabZLIB)
check_and_grab_zlib()

#
# curl package stuff
#
set(CURL_LINK)
find_package(CURL)
if (CURL_FOUND)
    SET(HAVE_CURL_H ON)

    if (USE_CURL)
        include_directories(${CURL_INCLUDE_DIR})
        SET(CURL_LINK "-lcurl")
    endif()
else()

endif()

if (DISABLE_AUTO_UPDATE)
    add_compile_options(-DDISABLE_AUTO_UPDATE)
endif()

#
# lua
#
set(LUA_LINK)
find_package(Lua)
if (LUA_FOUND)
    set(HAVE_LUA_H TRUE)

    if (USE_LUA)
        if (LUA_VERSION_MINOR LESS 3)
            set(LUA_LINK "-llua5.${LUA_VERSION_MINOR}")

            include_directories(${LUA_INCLUDE_DIR})
            add_compile_options(-DUSE_LUA)
        else()
            message(WARNING "Requires at least Lua 5.3, found Lua version ${LUA_VERSION_STRING}")
        endif()
    endif()
endif()

#set(THREADS_LINK)
#set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
#find_package(Threads)
#if (Threads_FOUND)
#    set(THREADS_LINK ${CMAKE_THREAD_LIBS_INIT})

#    set(ENABLE_THREADING TRUE)

#    if (CMAKE_USE_PTHREADS_INIT)
#        set(HAVE_PTHREAD_H TRUE)
#    endif()
#endif()

configure_file("${PROJECT_SOURCE_DIR}/config.h.in" 
    "${PROJECT_BINARY_DIR}/config.h")
include_directories(${PROJECT_BINARY_DIR})
add_compile_options(-DHAVE_CONFIG_H)

#
# headers
#
include_directories("src/external")
include_directories("src/include")
include_directories("src")

#
# compile options
# this needs to be before add_exectuable / add_library
#

# build options from q3
# -O3 -ffast-math -fomit-frame-pointer -fno-strict-aliasing
if (MSVC)
    # silence some warnings
    add_compile_options(/wd4244) # conversion from 'double' to 'float'
    add_compile_options(/wd4305) # truncation from 'double' to 'float'
    add_compile_options(/wd4996) # unsafe functions strcpy etc

    # winbase.h(9531): warning C5105: macro expansion producing 'defined' has undefined behavior
    add_compile_options(/wd5105)
else()
    add_compile_options(-Wall -Wextra -Wformat=2)

    if (NOT APPLE)
        add_compile_options(-Wformat-overflow=2)
    endif()
endif()

#
# main dedicated server
#
if (WIN32)
    add_executable(iocod WIN32 "")
else()
    add_executable(iocod "")
endif()

#
# game library 
#
add_library(game SHARED "")
target_compile_definitions(game PRIVATE GAME_DLL)

#
# main source directory
#
add_subdirectory("src")

#
# external libraries
#
add_subdirectory("external")

if (WIN32)
    include_directories("external/dlfcn-win32")
    include_directories("external/dirent")
    include_directories("external/getopt")
    include_directories("external/unistd")
endif()

#
# infostring
#
add_library(ic_infostring STATIC "")
target_sources(ic_infostring PRIVATE "src/infostring.c")
target_compile_options(ic_infostring PRIVATE -DINFOSTRING_STANDALONE)

#
# string library
#
add_library(ic_string STATIC "")
file(GLOB ic_string_sources "src/strings/*.c")
target_sources(ic_string PRIVATE ${ic_string_sources})

#
# cvar library for testing
#
add_library(ic_cvar STATIC "")
file(GLOB ic_cvar_sources "src/cvar/*.c")
list(FILTER ic_cvar_sources EXCLUDE REGEX ".*cvar_cmds.c")
target_sources(ic_cvar PRIVATE ${ic_cvar_sources})
target_compile_options(ic_cvar PRIVATE -DCVAR_STANDALONE)
target_link_libraries(ic_cvar ic_string ic_infostring)

#
# various options from above
#
if (BUILD_PRELOAD AND UNIX AND NOT COMPILE_64_BIT)
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
set(DLL_PLAT "x86_64")
set(DLL_EXT "so")
set(DLL_SPACER ".")

if (WIN32)
    if (NOT COMPILE_64_BIT)
        set(DLL_PLAT "x86")

        target_compile_options(iocod PRIVATE -DWIN32)
        target_compile_options(game PRIVATE -DWIN32)
        target_link_options(iocod PRIVATE -DWIN32)
        target_link_options(game PRIVATE -DWIN32)
    endif()

    set(DLL_SPACER "_")
    set(DLL_EXT "dll")
else()
    if (NOT COMPILE_64_BIT)
        set(DLL_PLAT "i386")

        target_compile_options(iocod PRIVATE -m32)
        target_compile_options(game PRIVATE -m32)
        target_link_options(iocod PRIVATE -m32)
        target_link_options(game PRIVATE -m32)
    endif()
endif()

set(DLL_OUT_NAME "game${DLL_SPACER}mp${DLL_SPACER}${DLL_PLAT}")

#
# any linux specific library linkage
#
if (WIN32)
    target_link_libraries(iocod ${CURL_LINK} ${LUA_LINK} ${THREADS_LINK} winmm)
    target_link_libraries(game ${CURL_LINK} ${LUA_LINK} ${THREADS_LINK} winmm)
else()
    target_link_libraries(iocod ${CMAKE_DL_LIBS} ${CURL_LINK} ${LUA_LINK} ${THREADS_LINK} m)
    target_link_libraries(game ${CURL_LINK} ${LUA_LINK} ${THREADS_LINK} m)
endif()
