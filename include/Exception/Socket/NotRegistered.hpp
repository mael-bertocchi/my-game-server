/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NotRegistered.hpp
*/

#pragma once

#include "Exception/Generic.hpp"
#include "Wrapper/Socket.hpp"

/**
 * @namespace Exception
 * @brief Contains exception-related classes and functions.
 */
namespace Exception::Socket
{
    /**
     * @class SocketNotRegisteredError
     * @brief Exception thrown when attempting to access a player that does not exist.
     */
    class NotRegisteredError : public Exception::GenericError
    {
        public:
            /**
             * @brief Create an error indicating that a player was not found.
             *
             * @param socket The socket identifier of the player that was not found.
             * @param where The location where the exception was thrown (default: current location).
             */
            explicit NotRegisteredError(const Wrapper::Socket::SocketType socket, const std::source_location &where = std::source_location::current());

        private:
            /**
             * @brief Format the error message with the provided type.
             *
             * @param socket The socket identifier of the player that was not found.
             * @return The formatted error message.
             */
            const std::string FormatError(const Wrapper::Socket::SocketType socket) const;
    };
}
