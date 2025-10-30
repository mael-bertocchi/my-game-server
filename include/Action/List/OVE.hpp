/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** OVE.hpp
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
     * @class OVE
     * @brief Action when the player moves
     */
    class OVE : public Action::AAction
    {
        public:
            /**
             * @brief Handle receiving a message from a session
             *
             * @param id The session identifier from which the message is received
             * @param body The body of the message received
             */
            void ReceiveMessage(const std::uint32_t id, const std::vector<std::uint8_t>& body) override;
    };
}
