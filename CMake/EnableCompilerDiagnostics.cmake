# SPDX-FileCopyrightText: 2022-2100 Amin Yahyaabadi 
# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: MIT
# SPDX-License-Identifier: GPL-3.0-or-later

include_guard()

if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    add_compile_options(-fcolor-diagnostics)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
else()
    message(STATUS "No colored diagnostics for ${CMAKE_CXX_COMPILER_ID}")
endif()