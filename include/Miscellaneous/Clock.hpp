/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Clock.hpp
*/

#pragma once

#include <chrono>

/**
 * @namespace Misc
 * @brief Contains miscellaneous utility classes and functions.
 */
namespace Misc
{
    /**
     * @class Clock
     * @brief A simple clock class to measure elapsed time.
     */
    class Clock
    {
        public:
            /**
             * @brief Create a new clock instance and start it.
             */
            Clock();

            /**
             * @brief Check if the specified duration has elapsed since the clock was started or last reset.
             *
             * @param duration The duration to check against.
             * @return True if the specified duration has elapsed, false otherwise.
             */
            bool HasElapsed(const std::uint32_t duration) const;

            /**
             * @brief Reset the clock to the current time.
             */
            void Reset();

            /**
             * @brief Get the elapsed time in seconds since the clock was started or last reset.
             *
             * @return The elapsed time in seconds as a float.
             */
            float GetElapsedTimeInSeconds() const;

        private:
            std::chrono::steady_clock::time_point _time; /*!< The start time of the clock */
    };
}
