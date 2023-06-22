// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <NullImplementation/NullClient.h>

namespace iocod {

NullClient client_local;
IClient* nullClient = &client_local;

void NullClient::Initialize() {}

void NullClient::Shutdown() {}

bool NullClient::ConsoleCommand()
{
    return false;
}

void NullClient::DrawActiveFrame(/* TODO */) {}

} // namespace iocod
