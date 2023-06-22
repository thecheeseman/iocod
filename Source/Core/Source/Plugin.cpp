// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/plugin.h>

#include <cstdio>
#include <cstring>

namespace iocod {

void GetPluginInterface(PluginInterface* interface)
{
    if (interface == nullptr)
        return;

    memset(interface, 0, interface->size);
}

} // namespace iocod
