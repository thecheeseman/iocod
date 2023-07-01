// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Core/System.h>
#include <game/game.h>

namespace iocod {

Game gameLocal;
IGame* game = &gameLocal;

//
// imports/exports
//
namespace {
GameExports exports;
}

ISystem* System;
IFileSystem* FileSystem;
ICommandSystem* CommandSystem;

extern "C" IOCOD_EXPORT GameExports* GetGameAPI(const GameImports* imports)
{
    if (imports->version == kGameAPIVersion) {
        System = imports->system;
        FileSystem = imports->file_system;
        CommandSystem = imports->command_system;
    }

    exports.game = game;

    return &exports;
}

//
// local
//

void Game::Initialize() {}

void Game::Shutdown() {}

void Game::ClientConnect(/* TODO */) {}

void Game::ClientBegin(/* TODO*/) {}

void Game::ClientUserInfoChanged(/* TODO */) {}

void Game::ClientDisconnect(/* TODO */) {}

void Game::ClientCommand(/* TODO */) {}

void Game::ClientThink(/* TODO */) {}

// not sure if these are strictly necessary
// void Game::GetFollowPlayerstate() {}
// void Game::UpdateCvars() {}

void Game::RunFrame() {}

bool Game::ConsoleCommand()
{
    return false;
}

void Game::ScriptFarHook(/* TODO */) {}

// again not sure about these
// void Game::CalcPose() {}
// void Game::ValidWeapon() {}
// void Game::SetPersistent() {}
// void Game::GetPersistent() {}
// void Game::GetClient2178() {}
// void Game::GetClientArchiveTime() {}
// void Game::SetClientArchiveTime() {}
// void Game::GetClientScore() {}

} // namespace iocod
