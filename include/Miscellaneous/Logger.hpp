/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Logger.hpp
*/

#pragma once

#include "Miscellaneous/Singleton.hpp"

#include <string>
#include <mutex>

/**
 * @namespace Misc
 * @brief Contains miscellaneous utility classes and functions.
 */
namespace Misc
{
    /**
     * @class Logger
     * @brief This class provides logging functionality with debug mode support.
     */
    class Logger : public Singleton<Logger>
    {
        public:
            /**
             * @enum LogLevel
             * @brief Represents the level of logging.
             */
            enum class LogLevel {
                Critical = 0, /*!< Error log level */
                Default = 1, /*!< Informational log level */
                Caution = 2 /*!< Warning log level */
            };

            /**
             * @brief Enable or disable debug mode
             *
             * @param enable True to enable debug mode, false to disable
             */
            void SetDebugMode(bool enable);

            /**
             * @brief Check if debug mode is enabled
             *
             * @return True if debug mode is enabled, false otherwise
             */
            bool IsDebugEnabled() const;

            /**
             * @brief WriteLogEntry a message with a timestamp
             *
             * @param message The message to log
             * @param level The level of the log message
             */
            void WriteLogEntry(const std::string& message, const LogLevel level = LogLevel::Default);

            /**
             * @brief Static convenience method to log a message
             *
             * @param message The message to log
             * @param level The level of the log message
             */
            static void Log(const std::string& message, const LogLevel level = LogLevel::Default);

        private:
            /**
             * @brief Private constructor for singleton pattern
             */
            Logger();

            /**
             * @brief Friend declaration for singleton access
             */
            friend class Singleton<Logger>;

            mutable std::mutex _mutex; /*!< Mutex for thread-safe logging */
            bool _enabled = false; /*!< Debug mode flag */
    };
}
