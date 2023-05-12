# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

message(STATUS "Configuring External Projects")
list(APPEND CMAKE_MESSAGE_INDENT "  ")

if (BUILD_TESTS)
    message(STATUS "Configuring Catch2")
    add_subdirectory("Catch2" EXCLUDE_FROM_ALL)
endif()

list(POP_BACK CMAKE_MESSAGE_INDENT)
list(POP_BACK CMAKE_MESSAGE_INDENT)
