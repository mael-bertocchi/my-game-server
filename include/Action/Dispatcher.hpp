/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Dispatch.hpp
*/

#pragma once

#include "Action/IAction.hpp"

#include <cstdint>
#include <any>

/**
 * @namespace Action
 * @brief Contains classes and functions related to actions.
 */
namespace Action
{
    /**
     * @class Dispatcher
     * @brief Dispatches messages to the appropriate action handlers based on message type.
     */
    class Dispatcher
    {
        public:
            /**
             * @brief Dispatch a received message to the appropriate action handler.
             *
             * @param type The type of the message.
             * @param id The player identifier from which the message is received.
             * @param body The body of the message received.
             */
            static void ReceiveMessage(const ActionType type, const std::uint32_t id, const std::vector<std::uint8_t>& body);

            /**
             * @brief Dispatch a message to be sent to the appropriate action handler.
             *
             * @param type The type of the message.
             * @param id The player identifier to which the message is sent.
             * @param content The content to be sent (default is empty).
             */
            static void SendMessage(const ActionType type, const std::uint32_t id, const std::any& content = {});

        private:
            /**
             * @brief Get the action handler for a given message type.
             *
             * @param type The type of the message.
             * @return A pointer to the static action handler, or nullptr if not found.
             */
            static Action::IAction* GetAction(const ActionType type);
    };
}
