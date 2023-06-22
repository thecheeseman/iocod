// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <NullImplementation/NullServer.h>

namespace iocod {

NullServer server_local;
IServer* nullServer = &server_local;

void NullServer::Initialize() {}

void NullServer::Shutdown() {}

void NullServer::RunFrame(int msec) {}

void NullServer::PacketEvent(/* TODO */) {}

bool NullServer::ConsoleCommand()
{
    return false;
}

} // namespace iocod
