/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AlreadyRegistered.hpp
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
     * @class AlreadyRegisteredError
     * @brief Exception class for handling already registered socket errors.
     */
    class AlreadyRegisteredError : public Exception::GenericError
    {
        public:
            /**
             * @brief Create an error indicating that a player is already connected.
             *
             * @param socket The socket identifier of the player that is already connected.
             * @param where The location where the exception was thrown (default: current location).
             */
            explicit AlreadyRegisteredError(const Wrapper::Socket::SocketType socket, const std::source_location &where = std::source_location::current());

        private:
            /**
             * @brief Format the error message with the provided type.
             *
             * @param socket The socket identifier of the player that is already connected.
             * @return The formatted error message.
             */
            const std::string FormatError(const Wrapper::Socket::SocketType socket) const;
    };
}
