// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/platform.h>
#include <server/server.h>

namespace iocod {

Server server_local;
IServer* server = &server_local;

//
// imports/exports
//
namespace {
ServerExports exports;
}

ISystem* System;
IFileSystem* FileSystem;
ICommandSystem* CommandSystem;

extern "C" IOCOD_EXPORT ServerExports* GetServerAPI(ServerImports* imports)
{
    if (imports->version == server_api_version) {
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
