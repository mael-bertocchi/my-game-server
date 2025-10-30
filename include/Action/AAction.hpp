/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AAction.hpp
*/

#pragma once

#include "Action/IAction.hpp"

/**
 * @namespace Action
 * @brief Namespace containing action-related interfaces and classes
 */
namespace Action
{
    /**
     * @class AAction
     * @brief Abstract base class for actions implementing common functionality
     *
     * @extends IAction
     */
    class AAction : public IAction
    {
        public:
            /**
             * @brief Virtual destructor to ensure proper cleanup of derived classes
             */
            virtual ~AAction() = default;

            /**
             * @brief Handle receiving a message from a
             *
             * @param id The identifier from which the message is received
             * @param body The body of the message received
             */
            void ReceiveMessage(const std::uint32_t id, const std::vector<std::uint8_t>& body) override;

            /**
             * @brief Handle sending a message to a
             *
             * @param id The identifier to which the message is sent
             * @param content The content of the message to be sent
             */
            void SendMessage(const std::uint32_t id, const std::any& content) const override;

            /**
             * @brief Validate a player based on specified flags
             *
             * @param player The player to validate
             * @param flags Bitwise OR combination of PlayerValidationFlags
             * @return An optional string containing an error message if validation fails, nullopt
             */
            std::optional<std::string> ValidatePlayer(const std::shared_ptr<Network::Player>& player, PlayerValidation flags = PlayerValidation::None) const override final;

            /**
             * @brief Validate a game based on specified flags
             *
             * @param game The game to validate
             * @param flags Bitwise OR combination of GameValidationFlags
             * @return An optional string containing an error message if validation fails, nullopt
             */
            std::optional<std::string> ValidateGame(const std::shared_ptr<Engine::Game>& game, GameValidation flags = GameValidation::None) const override final;
    };
}
