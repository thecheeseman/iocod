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

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(color_terminal)
setup_color_terminal()

#
# disable in-source builds
#
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "${Red}In-source builds are not allowed${ColorReset}")
endif()

#
# build type
#
if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug 
        CACHE STRING
        "Valid build types are: None, Debug, Release, RelWithDebInfo, MinSizeRel"
        FORCE
    )
endif()
message(STATUS "${Cyan}Build type: ${CMAKE_BUILD_TYPE}${ColorReset}")

#
# grab version string/numbering
#
include(get_version_info)
get_version_info()

#
# project
#
project(iocod 
    VERSION "${_version_string}"
    DESCRIPTION "open source CoD1.1-1.5 project"
    HOMEPAGE_URL "https://iocod.org"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS TRUE)

#
# check compiler/features/OS, etc.
#
include(check_features)
check_features()

#
# check headers etc
#


#
# superbuild
#
set_property(DIRECTORY PROPERTY EP_BASE ${CMAKE_BINARY_DIR}/deps)
set(STAGED_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/staged)
message(STATUS "${PROJECT_NAME} staged install: ${STAGED_INSTALL_PREFIX}")

configure_file("config.h.in" "${STAGED_INSTALL_PREFIX}/include/config.h")
include_directories(${STAGED_INSTALL_PREFIX})

# copy all relevant cache variables (IC_) to external project
get_cmake_property(_cache_vars CACHE_VARIABLES)
set(IC_CACHE_ARGS "")
foreach(_cv ${_cache_vars})
    string(SUBSTRING ${_cv} 0 3 _sub)
    if (${_sub} STREQUAL "IC_")
        get_property(_cvtype CACHE ${_cv} PROPERTY TYPE)
        if (_cvtype STREQUAL "UNINITIALIZED")
            set(_cvtype)
        else()
            set(_cvtype :${_cvtype})
        endif()

        list(APPEND IC_CACHE_ARGS "-D${_cv}${_cvtype}=${${_cv}}")
    endif()
endforeach()
message(DEBUG "IC_CACHE_ARGS: ${IC_CACHE_ARGS}")

#add_subdirectory(external)

if (SUPERBUILD)
    include(ExternalProject)
    ExternalProject_Add(${PROJECT_NAME}_core
        #DEPENDS
        #    miniz_external
        SOURCE_DIR 
            ${CMAKE_CURRENT_LIST_DIR}/src
        CMAKE_ARGS
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_C_STANDARD=${CMAKE_C_STANDARD}
            -DCMAKE_C_STANDARD_REQUIRED=${CMAKE_C_STANDARD_REQUIRED}
            -DCMAKE_C_EXTENSIONS=${CMAKE_C_EXTENSIONS}
        CMAKE_CACHE_ARGS
            -DSTAGED_INSTALL_PREFIX:STRING=${STAGED_INSTALL_PREFIX}
            -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
            -DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
            ${IC_CACHE_ARGS}
        BUILD_ALWAYS 
            ON
        INSTALL_COMMAND
            ""
    )
else()
    add_subdirectory("src")
endif()
