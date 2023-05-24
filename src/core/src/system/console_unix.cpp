// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <core/assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#include "console.h"

#define NO_LEAKS
#include <term.h>

namespace iocod {

static struct termios tc;
static std::size_t hide = 0;

// --------------------------------
// Console::Initialize
// --------------------------------
std::pair<bool, String> Console::Initialize() noexcept
{
    // TODO
    buffer.fill(0);

    return {true, ""};
}

// --------------------------------
// Console::impl::Shutdown
// --------------------------------
bool Console::impl::Shutdown() noexcept
{
    // TODO
    return true;
}

// --------------------------------
// Console::GetInput
// --------------------------------
String Console::GetInput() noexcept
{
    return {};
}

// --------------------------------
// Console::Back
// --------------------------------
void Console::Back()
{
    char key = '\b';
    write(STDOUT_FILENO, &key, 1);
    key = ' ';
    write(STDOUT_FILENO, &key, 1);
    key = '\b';
    write(STDOUT_FILENO, &key, 1);
}

// --------------------------------
// Console::Show
// --------------------------------
void Console::Show()
{
    if (!console_on)
        return;

    IOCOD_ASSERT(hide > 0);
    hide--;
    if (hide > 0)
        return;

    write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

    if (cursor > 0) {
        for (std::size_t i = 0; i < cursor; i++)
            write(STDOUT_FILENO, &buffer[i], 1);
    }
}

// --------------------------------
// Console::Hide
// --------------------------------
void Console::Hide() noexcept
{
    if (!console_on)
        return;

    if (hide > 0) {
        hide++;
        return;
    }

    if (cursor > 0) {
        for (std::size_t i = 0; i < cursor; i++)
            Back();
    }

    for (std::size_t i = strlen(PROMPT); i > 0; i--)
        Back();

    hide++;
}

} // namespace iocod
