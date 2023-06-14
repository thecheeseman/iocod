# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

message(STATUS "Configuring External Projects")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

if (BUILD_TESTS)
    message(STATUS "Configuring Catch2")
    add_subdirectory("Catch2" EXCLUDE_FROM_ALL)
endif()

# don't build shared lib for Catch2 
#set(BUILD_SHARED_LIBS TRUE)

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

#set(BUILD_SHARED_LIBS OFF)

list(POP_BACK CMAKE_MESSAGE_INDENT)
