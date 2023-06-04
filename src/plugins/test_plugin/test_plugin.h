// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TEST_PLUGIN_H
#define TEST_PLUGIN_H

#include <core/plugin.h>

#if 0

extern PluginInterface interface;

PLUGIN_EXPORT PluginInfo* PluginInitialize(GetInterfacePtr get_interface);
PLUGIN_EXPORT void PluginShutdown(void);

#endif

#endif // TEST_PLUGIN_H
