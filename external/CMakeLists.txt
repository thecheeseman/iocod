# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

message(STATUS "Configuring External Projects")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

if (BUILD_TESTS)
    message(STATUS "Configuring Catch2")
    add_subdirectory("Catch2" EXCLUDE_FROM_ALL)
endif()

message(STATUS "Configuring fmt")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
add_subdirectory("fmt" EXCLUDE_FROM_ALL)
list(POP_BACK CMAKE_MESSAGE_INDENT)

message(STATUS "Configuring spdlog")
set(SPDLOG_FMT_EXTERNAL ON)
list(APPEND CMAKE_MESSAGE_INDENT "  ")
add_subdirectory("spdlog" EXCLUDE_FROM_ALL)
list(POP_BACK CMAKE_MESSAGE_INDENT)

message(STATUS "Configuring miniz")
list(APPEND CMAKE_MESSAGE_INDENT "  ")
add_subdirectory("miniz" EXCLUDE_FROM_ALL)
list(POP_BACK CMAKE_MESSAGE_INDENT)

list(POP_BACK CMAKE_MESSAGE_INDENT)
