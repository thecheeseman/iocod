// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NULL_IMPL_NULL_CLIENT_H
#define NULL_IMPL_NULL_CLIENT_H

#include <core/iclient.h>

namespace iocod {

class NullClient final : public IClient {
public:
    void Initialize() override;
    void Shutdown() override;

    bool ConsoleCommand() override;
    void DrawActiveFrame(/* TODO */) override;
};

extern IClient* null_client; // fallback if we don't have a client DLL

} // namespace iocod

#endif // NULL_IMPL_NULL_CLIENT_H
