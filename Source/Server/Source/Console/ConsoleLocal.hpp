// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CONSOLE_CONSOLELOCAL_HPP
#define CONSOLE_CONSOLELOCAL_HPP

#include <Console.hpp>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <fcntl.h>
    #include <signal.h>
    #include <stdio.h>
    #include <sys/select.h>
    #include <termios.h>
    #include <unistd.h>

    #define NO_LEAKS
    #include <term.h>
#endif

#ifdef _WIN32
struct ConsoleWindows {
    HANDLE hin = nullptr;
    HANDLE hout = nullptr;
    DWORD original_mode = 0;
    WORD attributes = 0;
    WORD bg_attributes = 0;

    std::size_t cursor_pos = 0;
    std::size_t line_length = 0;
};

WORD Q3ColorToAttribute(Q3Color color) noexcept;

extern ConsoleWindows console;
#else
struct ConsoleLinux {
    struct termios tc;
    std::size_t hide = 0;
};

extern ConsoleLinux console;
#endif

#endif // CONSOLE_CONSOLELOCAL_HPP
