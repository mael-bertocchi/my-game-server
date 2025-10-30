/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SPW.hpp
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
     * @class SPW
     * @brief Action to spawn an entity in the game
     */
    class SPW : public Action::AAction
    {
        public:
            /**
             * @brief Handle sending a message to a session
             *
             * @param id The session identifier to which the message is sent
             * @param content The content to be sent, including entity type, identifier, and position
             */
            void SendMessage(const std::uint32_t id, const std::any& content) const override;
    };
}
