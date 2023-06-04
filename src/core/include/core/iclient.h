// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_ICLIENT_H
#define CORE_ICLIENT_H

namespace iocod {

// client DLL interface
class IClient {
public:
    virtual ~IClient() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual bool ConsoleCommand() = 0;
    virtual void DrawActiveFrame(/* TODO */) = 0;

    // other stuff??
};

extern IClient* client; // returned by the client DLL after initialization

//
// imports / exports
//
inline constexpr int client_api_version = 1;

#include "forward_decls.h"

struct ClientImports {
    int version{client_api_version};

    ISystem* system{nullptr};
    IFileSystem* file_system{nullptr};
    ICommandSystem* command_system{nullptr};
};

struct ClientExports {
    int version{client_api_version};

    IClient* client{nullptr};
};

using GetClientAPIPtr = ClientExports* (*)(ClientImports*);

} // namespace iocod

#endif //CORE_ICLIENT_H
