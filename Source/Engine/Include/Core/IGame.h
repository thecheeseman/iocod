// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_IGAME_H
#define CORE_IGAME_H

namespace iocod {

// game DLL interface
class IGame {
public:
    virtual ~IGame() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual void ClientConnect(/* TODO */) = 0;
    virtual void ClientBegin(/* TODO*/) = 0;
    virtual void ClientUserInfoChanged(/* TODO */) = 0;
    virtual void ClientDisconnect(/* TODO */) = 0;
    virtual void ClientCommand(/* TODO */) = 0;
    virtual void ClientThink(/* TODO */) = 0;

    // not sure if these are strictly necessary
    //virtual void GetFollowPlayerstate() = 0;
    //virtual void UpdateCvars() = 0;

    virtual void RunFrame() = 0;
    virtual bool ConsoleCommand() = 0;

    virtual void ScriptFarHook(/* TODO */) = 0;

    // again not sure about these
    //virtual void CalcPose() = 0;
    //virtual void ValidWeapon() = 0;
    //virtual void SetPersistent() = 0;
    //virtual void GetPersistent() = 0;
    //virtual void GetClient2178() = 0;
    //virtual void GetClientArchiveTime() = 0;
    //virtual void SetClientArchiveTime() = 0;
    //virtual void GetClientScore() = 0;
};

extern IGame* game; // returned by the game DLL after initialization

//
// import / exports
//
inline constexpr int kGameAPIVersion = 1;

#include "ForwardDecls.h"

struct GameImports {
    int version{kGameAPIVersion};

    ISystem* system{nullptr};
    IFileSystem* file_system{nullptr};
    ICommandSystem* command_system{nullptr};
};

struct GameExports {
    int version{kGameAPIVersion};

    IGame* game{nullptr};
};

using GetGameAPIPtr = GameExports* (*)(GameImports*);

} // namespace iocod

#endif // CORE_IGAME_H
