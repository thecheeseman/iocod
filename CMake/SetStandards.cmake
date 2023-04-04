# SPDX-FileCopyrightText: 2023 thecheeseman <thecheeseman@duck.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later

#
# C++ Standards
# require C++17 or later
#
list(FIND CMAKE_CXX_COMPILE_FEATURES "cxx_std_20" HAS_CXX20)
if (NOT HAS_CXX20)
    list(FIND CMAKE_CXX_COMPILE_FEATURES "cxx_std_17" HAS_CXX17)
    if (NOT HAS_CXX17)
        message(SEND_ERROR "C++17 required at least")
    endif()

    message(WARNING "C++20 not supported, using C++17")
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
    set(CMAKE_CXX_EXTENSIONS FALSE)
else()
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
    set(CMAKE_CXX_EXTENSIONS FALSE)
endif()

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