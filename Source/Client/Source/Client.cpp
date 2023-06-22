// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Client/Client.h>
#include <Core/Platform.h>

namespace iocod {

Client clientLocal;
IClient* client = &clientLocal;

//
// imports/exports
//
namespace {
ClientExports exports;
}

ISystem* System;
IFileSystem* FileSystem;
ICommandSystem* CommandSystem;

extern "C" IOCOD_EXPORT ClientExports* GetClientAPI(const ClientImports* imports)
{
    if (imports->version == kClientAPIVersion) {
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
