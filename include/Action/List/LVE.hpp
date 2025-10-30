/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LVE.hpp
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
     * @class LVE
     * @brief Action when the player leaves a game
     */
    class LVE : public Action::AAction
    {
        public:
            /**
             * @brief Handle receiving a message from a session
             *
             * @param id The session identifier from which the message is received
             * @param body The body of the message received
             */
            void ReceiveMessage(const std::uint32_t id, const std::vector<std::uint8_t>& body) override;

            /**
             * @brief Handle sending a message to a session
             *
             * @param id The session identifier to which the message is sent
             * @param content The content to be sent
             */
            void SendMessage(const std::uint32_t id, const std::any& content) const override;
    };
}
