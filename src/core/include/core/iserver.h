// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_ISERVER_H
#define CORE_ISERVER_H

namespace iocod {

// server DLL interface
class IServer {
public:
    virtual ~IServer() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual void RunFrame(int msec) = 0;
    virtual void PacketEvent(/* TODO */) = 0;

    virtual bool ConsoleCommand() = 0;
};

extern IServer* server; // returned by the server DLL after initialization

//
// imports / exports
//
inline constexpr int server_api_version = 1;

#include "forward_decls.h"

struct ServerImports {
    int version{server_api_version};

    ISystem* system{nullptr};
    IFileSystem* file_system{nullptr};
    ICommandSystem* command_system{nullptr};
};

struct ServerExports {
    int version{server_api_version};

    IServer* server{nullptr};
};

using GetServerAPIPtr = ServerExports* (*)(ServerImports*);

} // namespace iocod

#endif //CORE_ISERVER_H
