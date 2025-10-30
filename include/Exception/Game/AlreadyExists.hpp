/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AlreadyExists.hpp
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
     * @class AlreadyExistsError
     * @brief Exception class for handling already existing game errors.
     */
    class AlreadyExistsError : public Exception::GenericError
    {
        public:
            /**
             * @brief Create an error indicating that the game already exists.
             *
             * @param id The identifier of the entity that already exists.
             * @param where The location where the exception was thrown (default: current location).
             */
            explicit AlreadyExistsError(const std::uint32_t id, const std::source_location &where = std::source_location::current());

        private:
            /**
             * @brief Format the error message with the provided type.
             *
             * @param id The identifier of the entity that already exists.
             * @return The formatted error message.
             */
            const std::string FormatError(const std::uint32_t id) const;
    };
}
