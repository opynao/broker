#pragma once
#include <chrono>

template <class TimeT = std::chrono::seconds,
          class ClockT = std::chrono::high_resolution_clock >
          //steady clock
struct profiler
{
    template <class F, class... Args>
    static auto duration(F &&func, Args &&...args)
    {
        auto start = ClockT::now();
        std::invoke(std::forward<F>(func), std::forward<Args>(args)...);
        return std::chrono::duration_cast<TimeT>(ClockT::now() - start);
    }
};