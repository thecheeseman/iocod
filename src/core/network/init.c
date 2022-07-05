/*
================================================================================
iocod
Copyright (C) 2022 thecheeseman

This file is part of the iocod GPL source code.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
================================================================================
*/

#include "net_local.h"

socket_t ip_socket;
socket_t ip6_socket;

struct cvar *net_enabled;
struct cvar *net_ip;
struct cvar *net_ip6;
struct cvar *net_port;
struct cvar *net_port6;

#ifdef IC_PLATFORM_WINDOWS
WSADATA winsockdata = { 0 };
bool winsock_init = false;
#endif

static bool networking_enabled = false;

// ioq3
static bool net_update_cvars(void)
{
    int modified = 0;

    #define UPDATE(cv, v, f) do { \
        cv = cv_get(#cv, v, f); \
        modified = cv->modified; \
        cv->modified = false; \
    } while (0);

    UPDATE(net_enabled, "3", CV_LATCH | CV_ARCHIVE);
    UPDATE(net_ip, "0.0.0.0", CV_LATCH);
    UPDATE(net_ip6, "::", CV_LATCH);
    UPDATE(net_port, va("%i", PORT_SERVER), CV_LATCH);
    UPDATE(net_port6, va("%i", PORT_SERVER), CV_LATCH);
    #undef UPDATE_M

    static bool firsttime = true;
    if (firsttime) {
        firsttime = false;

        // only bother setting the cvar descriptions the first time
        cv_set_description(net_enabled, _("Enable networking."));
        cv_set_description(net_ip, _("IP address to bind to (IPv4)"));
        cv_set_description(net_ip6, _("IP address to bind to (IPv6)"));
        cv_set_description(net_port, _("Port to bind to (IPv4)"));
        cv_set_description(net_port6, _("Port to bind to (IPv6)"));
    }

    return modified > 0 ? true : false;
}

// ioq3
static void net_config(bool enable)
{
    bool modified = net_update_cvars();
    bool start;
    bool stop;

    // shouldn't be possible on dedicated but ok
    if (net_enabled->integer == 0)
        enable = false;

    // if enable state is the same and no cvars were modified,
    // nothing to do
    if (enable == networking_enabled && !modified)
        return;

    if (enable == networking_enabled) {
        if (enable) {
            stop = true;
            start = true;
        } else {
            stop = false;
            start = false;
        }
    } else {
        if (enable) {
            stop = false;
            start = true;
        } else {
            stop = true;
            start = false;
        }

        networking_enabled = enable;
    }

    if (stop) {
        if (ip_socket != INVALID_SOCKET) {
            closesocket(ip_socket);
            ip_socket = INVALID_SOCKET;
        }

        if (ip6_socket != INVALID_SOCKET) {
            closesocket(ip6_socket);
            ip6_socket = INVALID_SOCKET;
        }
    }

    if (start) {
        if (net_enabled->integer > 0)
            net_open_ip();
    }
}

IC_PUBLIC
void net_init(void)
{
    #ifdef IC_PLATFORM_WINDOWS
    int r = WSAStartup(MAKEWORD(1, 1), &winsockdata);
    if (r != 0) {
        log_error("WSAStartup: %s", net_error_string());
        return;
    }

    winsock_init = true;
    log_print(_("Winsock initialized"));
    #endif

    net_config(true);

    log_print("Network initialized");
}

IC_PUBLIC
void net_shutdown(void)
{
    if (!networking_enabled)
        return;

    log_debug("Network shutdown");

    net_config(false);

    #ifdef IC_PLATFORM_WINDOWS
    if (!winsock_init)
        return;

    WSACleanup();
    winsock_init = false;
    #endif
}

IC_PUBLIC
void net_restart(void)
{
    log_debug("Network restart");
    net_config(true);
}
