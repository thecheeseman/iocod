// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <core/iserver.h>

namespace iocod {

class Server final : public IServer {
public:
    void Initialize() override;
    void Shutdown() override;

    void RunFrame(int msec) override;
    void PacketEvent(/* TODO */) override;

    bool ConsoleCommand() override;

private:
    // other impl details
};

} // namespace iocod

#endif // SERVER_SERVER_H
