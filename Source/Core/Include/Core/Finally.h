// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_FINALLY_H
#define CORE_FINALLY_H

namespace iocod {

/// @brief Wrapper for a function that is executed when the object goes out of scope.
/// @tparam Functor Type of the function object.
/// Borrowed & modified from https://www.cppstories.com/2017/04/finalact/ / GSL
template <typename Functor>
class finally {
public:
    explicit finally(const Functor& f) noexcept
        : func_(f)
    {
    }

    explicit finally(Functor&& f) noexcept
        : func_(std::move(f))
    {
    }

    finally(finally&& other) noexcept
        : func_(std::move(other.func_))
    {
        other.cancel();
    }

    finally(const finally&) = delete;
    finally& operator=(const finally&) = delete;
    finally& operator=(finally&&) = delete;

    ~finally() noexcept
    {
        if (invoke_)
            func_();
    }

    /// @brief Cancels the function object.
    inline void cancel() noexcept
    {
        invoke_ = false;
    }

    /// @brief Executes the function object immediately.
    inline void execute() noexcept
    {
        if (invoke_)
            func_();

        cancel();
    }

private:
    Functor func_;
    bool invoke_ = true;
};

/// @brief Creates a finally object.
/// @tparam Functor Type of the function object.
/// @param f Function object.
/// @return finally<Functor> object.
template <typename Functor>
auto make_finally(Functor&& f) noexcept
{
    return finally<Functor>(std::forward<Functor>(f));
}

} // namespace iocod

#endif // CORE_FINALLY_H
