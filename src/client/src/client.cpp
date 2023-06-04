// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <client/client.h>
#include <core/platform.h>

namespace iocod {

Client client_local;
IClient* client = &client_local;

//
// imports/exports
//
namespace {
ClientExports exports;
}

ISystem* System;
IFileSystem* FileSystem;
ICommandSystem* CommandSystem;

extern "C" IOCOD_EXPORT ClientExports* GetClientAPI(ClientImports* imports)
{
    if (imports->version == client_api_version) {
        System = imports->system;
        FileSystem = imports->file_system;
        CommandSystem = imports->command_system;
    }

    exports.client = client;

    return &exports;
}

//
// local
//

void Client::Initialize() {}

void Client::Shutdown() {}

bool Client::ConsoleCommand()
{
    return false;
}

void Client::DrawActiveFrame(/* TODO */) {}

} // namespace iocod
