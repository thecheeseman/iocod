// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NULL_IMPL_NULL_SERVER_H
#define NULL_IMPL_NULL_SERVER_H

#include <core/iserver.h>

namespace iocod {

class NullServer final : public IServer {
public:
    void Initialize() override;
    void Shutdown() override;

    void RunFrame(int msec) override;
    void PacketEvent(/* TODO */) override;

    bool ConsoleCommand() override;
};

extern IServer* null_server; // fallback if we don't have a server DLL

} // namespace iocod

#endif // NULL_IMPL_NULL_SERVER_H
