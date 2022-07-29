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

project(miniz LANGUAGES C CXX)

include_directories("${STAGED_INSTALL_PREFIX}/include")
include_directories("../include")
include_directories(${PROJECT_BINARY_DIR})
link_directories("${STAGED_INSTALL_PREFIX}/lib")

add_library(miniz STATIC "miniz.c")

set_target_properties(miniz PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${STAGED_INSTALL_PREFIX}/lib"
)

