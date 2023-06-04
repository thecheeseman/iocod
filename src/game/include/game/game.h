// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GAME_GAME_H
#define GAME_GAME_H

#include <core/igame.h>

namespace iocod {

class Game final : public IGame {
public:
    void Initialize() override;
    void Shutdown() override;

    void ClientConnect(/* TODO */) override;
    void ClientBegin(/* TODO*/) override;
    void ClientUserInfoChanged(/* TODO */) override;
    void ClientDisconnect(/* TODO */) override;
    void ClientCommand(/* TODO */) override;
    void ClientThink(/* TODO */) override;

    // not sure if these are strictly necessary
    // void GetFollowPlayerstate() override;
    // void UpdateCvars() override;

    void RunFrame() override;
    bool ConsoleCommand() override;

    void ScriptFarHook(/* TODO */) override;

    // again not sure about these
    // void CalcPose() override;
    // void ValidWeapon() override;
    // void SetPersistent() override;
    // void GetPersistent() override;
    // void GetClient2178() override;
    // void GetClientArchiveTime() override;
    // void SetClientArchiveTime() override;
    // void GetClientScore() override;

private:
    // other impl details
};

} // namespace iocod

#endif // GAME_GAME_H
