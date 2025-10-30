/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EmptyMessageQueue.hpp
*/

#pragma once

#include "Exception/Generic.hpp"

/**
 * @namespace Exception
 * @brief Contains exception-related classes and functions.
 */
namespace Exception
{
    /**
     * @class EmptyMessageQueueError
     * @brief Exception thrown when attempting to add a player that is already connected.
     */
    class EmptyMessageQueueError : public Exception::GenericError
    {
        public:
            /**
             * @brief Create an error indicating that a player is already connected.
             *
             * @param where The location where the exception was thrown (default: current location).
             */
            explicit EmptyMessageQueueError(const std::source_location &where = std::source_location::current());
    };
}
