// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TestPlugin.h"

PluginInterface interface;
static PluginInfo info = {.version = IOCOD_VERSION_ENCODE(1, 0, 0),
                          .name = "TestPlugin",
                          .author = "thecheeseman",
                          .description = "some interesting description here"};

PLUGIN_EXPORT PluginInfo* PluginInitialize(GetInterfacePtr get_interface)
{
    interface.api = PLUGIN_API_VERSION;
    interface.size = sizeof(PluginInterface);
    get_interface(&interface);
    return &info;
}

PLUGIN_EXPORT void PluginShutdown(void) {}