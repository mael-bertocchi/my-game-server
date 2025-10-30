/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** STS.hpp
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
     * @class STS
     * @brief Action to notify players about statistic status changes
     */
    class STS : public Action::AAction
    {
        public:
            /**
             * @brief Handle sending a message to a player
             *
             * @param id The identifier to which the message is sent
             * @param content The content to be sent, including the the player identifier and whether to enable or disable the shield
             */
            void SendMessage(const std::uint32_t id, const std::any& content) const override;
    };
}
