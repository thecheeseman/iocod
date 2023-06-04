// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <null_impl/null_game.h>

namespace iocod {

NullGame game_local;
IGame* null_game = &game_local;

void NullGame::Initialize() {}

void NullGame::Shutdown() {}

void NullGame::ClientConnect(/* TODO */) {}

void NullGame::ClientBegin(/* TODO*/) {}

void NullGame::ClientUserInfoChanged(/* TODO */) {}

void NullGame::ClientDisconnect(/* TODO */) {}

void NullGame::ClientCommand(/* TODO */) {}

void NullGame::ClientThink(/* TODO */) {}

// not sure if these are strictly necessary
// void NullGame::GetFollowPlayerstate() {}
// void NullGame::UpdateCvars() {}

void NullGame::RunFrame() {}

bool NullGame::ConsoleCommand()
{
    return false;
}

void NullGame::ScriptFarHook(/* TODO */) {}

// again not sure about these
// void NullGame::CalcPose() {}
// void NullGame::ValidWeapon() {}
// void NullGame::SetPersistent() {}
// void NullGame::GetPersistent() {}
// void NullGame::GetClient2178() {}
// void NullGame::GetClientArchiveTime() {}
// void NullGame::SetClientArchiveTime() {}
// void NullGame::GetClientScore() {}

} // namespace iocod
