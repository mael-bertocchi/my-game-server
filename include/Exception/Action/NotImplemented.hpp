/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NotImplemented.hpp
*/

#pragma once

#include "Exception/Generic.hpp"

/**
 * @namespace Exception
 * @brief Contains exception-related classes and functions.
 */
namespace Exception::Action
{
    /**
     * @class EmptyMessageQueueError
     * @brief Exception thrown when attempting to add a player that is already connected.
     */
    class NotImplementedError : public Exception::GenericError
    {
        public:
            /**
             * @brief Create an error indicating that a player is already connected.
             *
             * @param type The type of the message that caused the error.
             * @param where The location where the exception was thrown (default: current location).
             */
            explicit NotImplementedError(const std::string& type, const std::source_location &where = std::source_location::current());

        private:
            /**
             * @brief Format the error message for an already registered player.
             *
             * @param type The type of the message that caused the error.
             * @return A formatted error message string.
             */
            const std::string FormatError(const std::string& type) const;
    };
}
