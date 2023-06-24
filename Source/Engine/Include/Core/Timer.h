// SPDX-FileCopyrightText: 2023 thecheeseman
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CORE_TIMER_H
#define CORE_TIMER_H

#include <chrono>
#include <Core/Assert.h>
#include <Core/Types.h>

namespace iocod {

using Milliseconds = std::chrono::milliseconds;
using Microseconds = std::chrono::microseconds;
using Nanoseconds = std::chrono::nanoseconds;

class Timer final {
public:
    enum class State {
        Stopped,
        Started
    };

    void Start()
    {
        Assert(m_state == State::Stopped);

        m_start = Clock::now();
        m_state = State::Started;
    }

    void Stop()
    {
        Assert(m_state == State::Started);

        m_end = Clock::now();
        m_state = State::Stopped;
    }

    void Clear()
    {
        m_start = {};
        m_end = {};
        m_state = State::Stopped;
    }

    [[nodiscard]] u64 Milliseconds() const
    {
        Assert(m_state == State::Stopped);

        return std::chrono::duration_cast<iocod::Milliseconds>(m_end - m_start).count();
    }

    [[nodiscard]] u64 Microseconds() const
    {
        Assert(m_state == State::Stopped);

        return std::chrono::duration_cast<iocod::Microseconds>(m_end - m_start).count();
    }

    [[nodiscard]] u64 Nanoseconds() const
    {
        Assert(m_state == State::Stopped);

        return std::chrono::duration_cast<iocod::Nanoseconds>(m_end - m_start).count();
    }

private:
    using Clock = std::chrono::steady_clock;
    using Time = std::chrono::time_point<Clock>;

    State m_state{State::Stopped};
    Time m_start{};
    Time m_end{};
};

} // namespace iocod

#endif // CORE_TIMER_H
