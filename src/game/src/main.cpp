// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/platform.h>
#include <game/game.h>

GameImports* g_imports = nullptr;

void init(int level_time, int random_seed, int restart, int)
{

}

void shutdown()
{

}

extern "C" IOCOD_EXPORT void EntryPoint(GameImports* imports, GameExports* exports)
{
    g_imports = imports;

    exports->size = sizeof(GameExports);
    exports->init = init;
    exports->shutdown = shutdown;
}
