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
                Caution = 2, /*!< Warning log level */
                Network = 3 /*!< Network request log level */
            };

            /**
             * @enum Verbosity
             * @brief Represents the verbosity level of logging.
             */
            enum class Verbosity {
                None = 0, /*!< No logging */
                Default = 1, /*!< Log everything except network requests */
                Network = 2 /*!< Log everything including network requests */
            };

            /**
             * @brief Set the logging verbosity level
             *
             * @param level The verbosity level to set
             */
            void SetVerbosity(Verbosity level);

            /**
             * @brief Get the current logging verbosity level
             *
             * @return The current verbosity level
             */
            Verbosity GetVerbosity() const;

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

            Verbosity _verbosity; /*!< Logging verbosity level */
    };
}
