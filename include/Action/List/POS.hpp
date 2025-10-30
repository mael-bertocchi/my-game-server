/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** POS.hpp
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
     * @class POS
     * @brief Action to broadcast the position of entities
     */
    class POS : public Action::AAction
    {
        public:
            /**
             * @brief Handle sending a message to a session
             *
             * @param id The session identifier to which the message is sent
             * @param content The content to be sent, must be a vector of position updates:
             *        vector<tuple<Entity, uint32_t, Position>>
             */
            void SendMessage(const std::uint32_t id, const std::any& content) const override;
    };
}
