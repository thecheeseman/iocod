// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include <core/iclient.h>

namespace iocod {

class Client final : public IClient {
public:
    void Initialize() override;
    void Shutdown() override;

    bool ConsoleCommand() override;
    void DrawActiveFrame(/* TODO */) override;

private:
    // other impl details
};

} // namespace iocod

#endif // CLIENT_CLIENT_H
