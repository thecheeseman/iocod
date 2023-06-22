// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <..\..\Core\Include\Core\Platform.h>
#include <server/server.h>

namespace iocod {

Server serverLocal;
IServer* server = &serverLocal;

//
// imports/exports
//
namespace {
ServerExports exports;
}

ISystem* System;
IFileSystem* FileSystem;
ICommandSystem* CommandSystem;

extern "C" IOCOD_EXPORT ServerExports* GetServerAPI(const ServerImports* imports)
{
    if (imports->version == kServerAPIVersion) {
        System = imports->system;
        FileSystem = imports->file_system;
        CommandSystem = imports->command_system;
    }

    exports.server = server;

    return &exports;
}

//
// local
//

void Server::Initialize() {}

void Server::Shutdown() {}

void Server::RunFrame(int msec) {}

void Server::PacketEvent(/* TODO */) {}

bool Server::ConsoleCommand()
{
    return false;
}

} // namespace iocod
