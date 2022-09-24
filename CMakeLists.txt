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

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

#
# disable in-source builds
#
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed")
endif()

#
# build type
#
if (NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo
        CACHE STRING
        "Valid build types are: None, Debug, Release, RelWithDebInfo, MinSizeRel"
        FORCE
    )
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(IOCOD_DEBUG TRUE)
endif()

#
# grab version string/numbering
#
#include(get_version_info)
#get_version_info()

#
project(iocod 
    # VERSION "${_version_string}"
    DESCRIPTION "open source CoD1 project"
    HOMEPAGE_URL "https://iocod.org"
    LANGUAGES CXX
)

#
# headers / compiler features 
#
include(check_headers)
check_headers()

include(check_features)
check_features()

include(multithreaded)

#include(cxx_modules)

#
#
#
set_property(DIRECTORY PROPERTY EP_BASE ${CMAKE_BINARY_DIR}/deps)
set(STAGED_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/staged)
message(STATUS "${PROJECT_NAME} staged install: ${STAGED_INSTALL_PREFIX}")

configure_file("src/iocod_config.h.in" "${STAGED_INSTALL_PREFIX}/include/iocod_config.h")
add_compile_definitions(HAVE_CONFIG_H)
include_directories("${STAGED_INSTALL_PREFIX}/include")

#
#
#
add_executable(iocod)
set_target_properties(iocod PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/bin"
)

if (IOCOD_PLATFORM_LINUX OR IOCOD_PLATFORM_MACOS)
    target_link_libraries(iocod PRIVATE dl)
endif()

#
add_subdirectory("external")

add_subdirectory("src")
