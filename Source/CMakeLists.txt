# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

configure_file("iocod_config.h.in" "${STAGED_INSTALL_PREFIX}/Include/iocod_config.h" @ONLY)

add_subdirectory("Core")
add_subdirectory("Hook")
add_subdirectory("Plugins")
add_subdirectory("Server")
