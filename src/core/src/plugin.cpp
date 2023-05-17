// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Core/Plugin.h>
#include <cstring>
#include <cstdio>

static void interface_log(const char* message)
{
    printf("PluginInterface->log: %s\n", message);
}

void GetPluginInterface(PluginInterface* interface) 
{
    if (interface == nullptr)
        return;

    memset(interface, 0, interface->size);

    const uint32_t api_major = IOCOD_VERSION_DECODE_MAJOR(interface->api);
    const uint32_t api_minor = IOCOD_VERSION_DECODE_MINOR(interface->api);
    const uint32_t api_patch = IOCOD_VERSION_DECODE_PATCH(interface->api);

    interface->log = interface_log;
}