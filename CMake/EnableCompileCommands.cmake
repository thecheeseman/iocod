# SPDX-FileCopyrightText: 2022-2100 Amin Yahyaabadi 
# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: MIT
# SPDX-License-Identifier: GPL-3.0-or-later

include_guard()

if (0)
if (CMAKE_GENERATOR MATCHES ".*Makefile.*" OR CMAKE_GENERATOR MATCHES ".*Ninja.*")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    file(CREATE_LINK 
        "${CMAKE_BINARY_DIR}/compile_commands.json"
        "${CMAKE_SOURCE_DIR}/compile_commands.json"
        SYMBOLIC
    )
    message(TRACE "Created symbolic link to compile_commands.json")
endif()
endif()
