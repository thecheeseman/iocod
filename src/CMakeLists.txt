# SPDX-FileCopyrightText: 2023 thecheeseman
#
# SPDX-License-Identifier: GPL-3.0-or-later

configure_file("iocod_config.h.in" "${STAGED_INSTALL_PREFIX}/include/iocod_config.h" @ONLY)

add_subdirectory(client)
add_subdirectory(core)
add_subdirectory(game)
add_subdirectory(iocod)
add_subdirectory(plugins)
add_subdirectory(server)
