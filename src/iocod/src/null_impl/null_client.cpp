// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <null_impl/null_client.h>

namespace iocod {

NullClient client_local;
IClient* null_client = &client_local;

void NullClient::Initialize() {}

void NullClient::Shutdown() {}

bool NullClient::ConsoleCommand()
{
    return false;
}

void NullClient::DrawActiveFrame(/* TODO */) {}

} // namespace iocod
