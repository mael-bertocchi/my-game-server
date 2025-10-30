/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** IAction.hpp
*/

#pragma once

#include "Network/Player.hpp"
#include "Engine/Game.hpp"

#include <optional>
#include <cstdint>
#include <vector>
#include <any>

/**
 * @namespace Action
 * @brief Namespace containing action-related interfaces and classes
 */
namespace Action
{
    /**
     * @interface IAction
     * @brief Interface for actions that can be performed on messages
     */
    class IAction
    {
        public:
            /**
             * @enum PlayerValidationFlags
             * @brief Bitwise flags for player validation options
             */
            enum class PlayerValidation : std::uint8_t {
                None = 0, /*!< No validation */
                Administrator = 1 << 0, /*!< Player must be an administrator */
                NotConnected = 1 << 1, /*!< Player must not be connected */
                NotPlaying = 1 << 2, /*!< Player must not be in a game */
                Connected = 1 << 3, /*!< Player must be connected */
                Playing = 1 << 4 /*!< Player must be in a game */
            };

            /**
             * @brief Bitwise OR operator for PlayerValidation
             *
             * @param lhs Left-hand side PlayerValidation
             * @param rhs Right-hand side PlayerValidation
             * @return Resulting PlayerValidation after bitwise OR operation
             */
            friend constexpr PlayerValidation operator|(PlayerValidation lhs, PlayerValidation rhs) {
                return static_cast<PlayerValidation>(static_cast<std::uint8_t>(lhs) | static_cast<std::uint8_t>(rhs));
            }

            /**
             * @brief Bitwise AND operator for PlayerValidation
             *
             * @param lhs Left-hand side PlayerValidation
             * @param rhs Right-hand side PlayerValidation
             * @return Resulting PlayerValidation after bitwise AND operation
             */
            friend constexpr PlayerValidation operator&(PlayerValidation lhs, PlayerValidation rhs) {
                return static_cast<PlayerValidation>(static_cast<std::uint8_t>(lhs) & static_cast<std::uint8_t>(rhs));
            }

            /**
             * @brief Inequality operator for PlayerValidation
             *
             * @param lhs Left-hand side PlayerValidation
             * @param rhs Right-hand side PlayerValidation
             * @return True if values are not equal
             */
            friend constexpr bool operator!=(PlayerValidation lhs, PlayerValidation rhs) {
                return static_cast<std::uint8_t>(lhs) != static_cast<std::uint8_t>(rhs);
            }

            /**
             * @enum GameValidationFlags
             * @brief Bitwise flags for game validation options
             */
            enum class GameValidation : std::uint8_t {
                None = 0, /*!< No validation */
                NotStarted = 1 << 0, /*!< Game must not have started */
                Started = 1 << 1, /*!< Game must have started */
                NotFull = 1 << 2, /*!< Game must not be full */
                Full = 1 << 3, /*!< Game must be full */
                Min = 1 << 4 /*!< Game must have minimum players */
            };

            /**
             * @brief Bitwise OR operator for GameValidation
             *
             * @param lhs Left-hand side GameValidation
             * @param rhs Right-hand side GameValidation
             * @return Resulting GameValidation after bitwise OR operation
             */
            friend constexpr GameValidation operator|(GameValidation lhs, GameValidation rhs) {
                return static_cast<GameValidation>(static_cast<std::uint8_t>(lhs) | static_cast<std::uint8_t>(rhs));
            }

            /**
             * @brief Bitwise AND operator for GameValidation
             *
             * @param lhs Left-hand side GameValidation
             * @param rhs Right-hand side GameValidation
             * @return Resulting GameValidation after bitwise AND operation
             */
            friend constexpr GameValidation operator&(GameValidation lhs, GameValidation rhs) {
                return static_cast<GameValidation>(static_cast<std::uint8_t>(lhs) & static_cast<std::uint8_t>(rhs));
            }

            /**
             * @brief Inequality operator for GameValidation
             *
             * @param lhs Left-hand side GameValidation
             * @param rhs Right-hand side GameValidation
             * @return True if values are not equal
             */
            friend constexpr bool operator!=(GameValidation lhs, GameValidation rhs) {
                return static_cast<std::uint8_t>(lhs) != static_cast<std::uint8_t>(rhs);
            }

            /**
             * @brief Virtual destructor to ensure proper cleanup of derived classes
             */
            virtual ~IAction() = default;

            /**
             * @brief Handle receiving a message from a
             *
             * @param id The identifier from which the message is received
             * @param body The body of the message received
             */
            virtual void ReceiveMessage(const std::uint32_t id, const std::vector<std::uint8_t>& body) = 0;

            /**
             * @brief Handle sending a message to a
             *
             * @param id The identifier to which the message is sent
             * @param content The content of the message to be sent
             */
            virtual void SendMessage(const std::uint32_t id, const std::any& content) const = 0;

            /**
             * @brief Validate a player based on specified flags
             *
             * @param player The player to validate
             * @param flags Bitwise OR combination of PlayerValidationFlags
             * @return An optional string containing an error message if validation fails, nullopt
             */
            virtual std::optional<std::string> ValidatePlayer(const std::shared_ptr<Network::Player>& player, PlayerValidation flags) const = 0;

            /**
             * @brief Validate a game based on specified flags
             *
             * @param game The game to validate
             * @param flags Bitwise OR combination of GameValidationFlags
             * @return An optional string containing an error message if validation fails, nullopt
             */
            virtual std::optional<std::string> ValidateGame(const std::shared_ptr<Engine::Game>& game, GameValidation flags) const = 0;
    };
}
