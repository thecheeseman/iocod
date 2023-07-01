// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_FILE_PATH_H
#define CORE_FILE_PATH_H

#include <algorithm>
#include <Core/Assert.h>
#include <Core/FixedString.h>
#include <Core/String.h>

namespace iocod {

class FilePath {
public:
    static inline constexpr size_t MAX_PATH = 256;

    FilePath() = default;
    ~FilePath() = default;

    explicit FilePath(const char* path_)
    {
        const std::size_t len = std::strlen(path_);
        Assert(len <= MAX_PATH, "Path is too long");
        std::copy(path_, path_ + (len > path.max_size() ? path.max_size() : len), path.data());
    }

    explicit FilePath(const String& path_)
    {
        Assert(path_.Length() <= MAX_PATH, "Path is too long");
        //std::copy(path_.begin(), path_.end(), path.data());
    }

private:
    FixedString<MAX_PATH> path{};
};

} // namespace iocod

#endif // CORE_FILE_PATH_H
