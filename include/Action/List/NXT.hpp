/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NXT.hpp
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
     * @class NXT
     * @brief Action to notify the player of the next wave
     */
    class NXT : public Action::AAction
    {
        public:
            /**
             * @brief Send the next wave action to the player
             *
             * @param id The identifier of the player
             * @param content The content of the action
             */
             void SendMessage(const std::uint32_t id, const std::any& content) const override;
    };
}
