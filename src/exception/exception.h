#pragma once

#include <string_view>
#include <utility>

class exception {
public:
    exception(std::string_view&& text = "")
    {
        error = std::move(text);
    }

    const std::string_view& get_error() const
    {
        return error;
    }

private:
    friend class fatal_exception;
    inline static std::string_view error;
};

class fatal_exception {
public:
    fatal_exception(std::string_view&& text = "")
    {
        exception::error = std::move(text);
    }

    const std::string_view& get_error() const
    {
        return exception::error;
    }
};
