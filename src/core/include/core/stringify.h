// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_STRINGIFY_H
#define CORE_STRINGIFY_H

/// @def IOCOD_STRINGIFY
/// @brief Stringifies a macro argument.
#define IOCOD_STRINGIFY_(x) #x
#define IOCOD_STRINGIFY(x) IOCOD_STRINGIFY_(x)

/// @def IOCOD_CONCAT
/// @brief Concatenates two macro arguments.
#define IOCOD_CONCAT_(a, b) a ## b
#define IOCOD_CONCAT(a, b) IOCOD_CONCAT_(a, b)

/// @def IOCOD_CONCAT3
/// @brief Concatenates three macro arguments.
#define IOCOD_CONCAT3_(a, b, c) a ## b ## c
#define IOCOD_CONCAT3(a, b, c) IOCOD_CONCAT3_(a, b, c)

#endif // CORE_STRINGIFY_H
