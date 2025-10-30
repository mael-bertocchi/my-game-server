/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NotExists.hpp
*/

#pragma once

#include "Exception/Generic.hpp"

#include <cstdint>

/**
 * @namespace Exception
 * @brief Contains exception-related classes and functions.
 */
namespace Exception::Game
{
    /**
     * @class SocketNotExistsError
     * @brief Exception thrown when attempting to access a player that does not exist.
     */
    class NotExistsError : public Exception::GenericError
    {
        public:
            /**
             * @brief Create an error indicating that a player was not found.
             *
             * @param id The identifier of the entity that does not exist.
             * @param where The location where the exception was thrown (default: current location).
             */
            explicit NotExistsError(const std::uint32_t id, const std::source_location &where = std::source_location::current());

        private:
            /**
             * @brief Format the error message with the provided type.
             *
             * @param id The identifier of the entity that does not exist.
             * @return The formatted error message.
             */
            const std::string FormatError(const std::uint32_t id) const;
    };
}
