# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

#
# C++ Standards
# require C++20 or later
#
list(FIND CMAKE_CXX_COMPILE_FEATURES "cxx_std_20" HAS_CXX20)
if (NOT HAS_CXX20)
    message(SEND_ERROR "C++20 required")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

#
# C Standards
#
list(FIND CMAKE_C_COMPILE_FEATURES "c_std_11" HAS_C11)
if (NOT HAS_C11)
    message(SEND_ERROR "C11 required")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS FALSE)
