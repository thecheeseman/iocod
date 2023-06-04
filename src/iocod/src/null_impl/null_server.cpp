// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <null_impl/null_server.h>

namespace iocod {

NullServer server_local;
IServer* null_server = &server_local;

void NullServer::Initialize() {}

void NullServer::Shutdown() {}

void NullServer::RunFrame(int msec) {}

void NullServer::PacketEvent(/* TODO */) {}

bool NullServer::ConsoleCommand()
{
    return false;
}

} // namespace iocod
