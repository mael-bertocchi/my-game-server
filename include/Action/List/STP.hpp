/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** STP.hpp
*/

#pragma once

#include "Action/AAction.hpp"

/**
 * @namespace Action
 * @brief Namespace containing action-related interfaces and classes
 */
namespace Action::List
{
    /**
     * @class STP
     * @brief Action when the the game is stopping
     */
    class STP : public Action::AAction
    {
        public:
            /**
             * @enum Result
             * @brief Possible results when the game stops
             */
            enum class Result : std::uint8_t {
                Lose = 0, /*!< The player lost the game */
                Win = 1 /*!< The player won the game */
            };

            /**
             * @brief Handle sending a message to a session
             *
             * @param id The session identifier to which the message is sent
             * @param content The content to be sent
             */
            void SendMessage(const std::uint32_t id, const std::any& content) const override;
    };
}
