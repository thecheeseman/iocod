// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_CLASS_NON_COPYABLE_H
#define CORE_CLASS_NON_COPYABLE_H

#define CLASS_NON_COPYABLE(Type)           \
    Type(const Type&) = delete;            \
    Type(Type&&) noexcept = delete;        \
    Type& operator=(const Type&) = delete; \
    Type& operator=(Type&&) noexcept = delete;

#endif // CORE_CLASS_NON_COPYABLE_H
