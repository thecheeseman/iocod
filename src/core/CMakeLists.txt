# iocod
# Copyright (C) 2022 thecheeseman
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

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(iocod_core LANGUAGES C)

file(GLOB IC_CORE_SRCS "*.c")

foreach (subdir
    commands
    common
    config
    console
    cvar
    events
    fs
    log
    network
    parse
    shared
    strings
    system)
    file(GLOB sources "${subdir}/*.c")
    list(APPEND IC_CORE_SRCS ${sources})
endforeach()

if (IC_PLATFORM_MSVC)
    # always silence these warnings
    add_compile_options(/wd4244) # conversion from 'double' to 'float'
    add_compile_options(/wd4305) # truncation from 'double' to 'float'
    add_compile_options(/wd4996) # unsafe functions strcpy etc

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        # instead of add_compile_options()
        # because this produces warning D9025
        string(REPLACE "/W3" "/W4" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
    endif()
elseif (IC_PLATFORM_GCC OR IC_PLATFORM_CLANG OR IC_PLATFORM_MINGW)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-Wall -Wextra)
    endif()
endif()

add_compile_definitions(HAVE_CONFIG_H)
include_directories("${STAGED_INSTALL_PREFIX}/include")
include_directories("../include")
include_directories(${PROJECT_BINARY_DIR})
link_directories("${STAGED_INSTALL_PREFIX}/lib")

add_library(iocod_core STATIC ${IC_CORE_SRCS})

set_target_properties(iocod_core PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/lib"
)

add_subdirectory("tests")
