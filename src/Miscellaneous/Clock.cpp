/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Clock.cpp
*/

#include "Miscellaneous/Clock.hpp"

Misc::Clock::Clock() : _time(std::chrono::steady_clock::now()) {}

bool Misc::Clock::HasElapsed(const std::uint32_t duration) const
{
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _time).count();

    return elapsed >= duration;
}

void Misc::Clock::Reset()
{
    _time = std::chrono::steady_clock::now();
}

float Misc::Clock::GetElapsedTimeInSeconds() const
{
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - _time).count();

    return elapsed / 1000000.0f;
}
