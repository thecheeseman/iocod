// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_VERSION_H
#define CORE_VERSION_H

/// @def IOCOD_VERSION_ENCODE
/// Encode version number into single value.
#define IOCOD_VERSION_ENCODE(major, minor, revision) \
    (((major) *1000000) + ((minor) *1000) + (revision))

/// @def IOCOD_VERSION_DECODE_MAJOR
/// Decode the major version from the full version value.
#define IOCOD_VERSION_DECODE_MAJOR(version) ((version) / 1000000)

/// @def IOCOD_VERSION_DECODE_MINOR
/// Decode the minor version from the full version value.
#define IOCOD_VERSION_DECODE_MINOR(version) ((version % 1000000) / 1000)

/// @def IOCOD_VERSION_DECODE_PATCH
/// Decode the patch number from the full version value.
#define IOCOD_VERSION_DECODE_PATCH(version) ((version) % 1000)

#endif // CORE_VERSION_H
