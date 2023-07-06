# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

message(STATUS "Configuring External Projects")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

message(STATUS "Configuring fmt")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
add_subdirectory("fmt")
list(POP_BACK CMAKE_MESSAGE_INDENT)

message(STATUS "Configuring spdlog")
set(SPDLOG_FMT_EXTERNAL ON)
list(APPEND CMAKE_MESSAGE_INDENT "  ")
add_subdirectory("spdlog")
list(POP_BACK CMAKE_MESSAGE_INDENT)

message(STATUS "Configuring miniz")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
add_subdirectory("miniz")
list(POP_BACK CMAKE_MESSAGE_INDENT)

message(STATUS "Configuring nanobench")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
add_library(nanobench STATIC "nanobench.cpp")
list(POP_BACK CMAKE_MESSAGE_INDENT)

message(STATUS "Configuring doctest")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
add_library(doctest STATIC "doctest.cpp")
list(POP_BACK CMAKE_MESSAGE_INDENT)

message(STATUS "Configuring EA modules")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
add_subdirectory(EABase)
add_subdirectory(EAAssert)
add_subdirectory(EAThread)
add_subdirectory(EAStdC)
target_sources(EAStdC PRIVATE "eastdc.cpp")
add_subdirectory(EASTL)
list(POP_BACK CMAKE_MESSAGE_INDENT)

message(STATUS "Configuring rpmalloc")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
add_library(rpmalloc STATIC "rpmalloc/rpmalloc/rpmalloc.c")
list(POP_BACK CMAKE_MESSAGE_INDENT)

list(POP_BACK CMAKE_MESSAGE_INDENT)
