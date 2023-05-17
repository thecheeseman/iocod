# SPDX-FileCopyrightText: 2022-2100 Amin Yahyaabadi 
# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: MIT
# SPDX-License-Identifier: GPL-3.0-or-later

include_guard()

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified")

    set(CMAKE_BUILD_TYPE
        RelWithDebInfo
        CACHE STRING "Choose the type of build" FORCE
    )

    # Set the possible values of build type for cmake-gui, ccmake
    set_property(
        CACHE CMAKE_BUILD_TYPE
        PROPERTY STRINGS
        "Debug"
        "Release"
        "MinSizeRel"
        "RelWithDebInfo"
    )
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEBUG_BUILD TRUE)
endif()