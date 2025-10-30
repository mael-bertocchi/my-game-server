/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Player.hpp
*/

#pragma once

#include "Miscellaneous/Clock.hpp"
#include "Wrapper/Socket.hpp"
#include "Types.hpp"

#include <unordered_map>
#include <cstdint>
#include <vector>
#include <queue>
#include <mutex>

/**
 * @namespace Network
 * @brief Contains classes and functions related to network operations.
 */
namespace Network
{
    /**
     * @class Player
     * @brief A class representing a player in the networked game.
     */
    class Player
    {
        public:
            /**
             * @struct Message
             * @brief Structure representing a message to be sent or received.
             */
            struct Message {
                ActionType type; /*!< The type of the message */
                std::vector<std::uint8_t> body; /*!< The content of the message */
            };

            /**
             * @brief Create a new player.
             *
             * @param address The address of the player
             * @param port The port number of the player
             */
            explicit Player(const std::string& address, const std::uint16_t port);

            /**
             * @brief Destroy the player and clean associated resources.
             */
            ~Player();

            /**
             * @brief Add a message to the player's message queue.
             *
             * @param protocol The protocol of the message queue to push to.
             * @param message The message to be added to the queue.
             */
            void PushMessage(const Wrapper::Socket::Protocol& protocol, const Message& message);

            /**
             * @brief Remove and return the oldest message from the player's message queue.
             *
             * @param protocol The protocol of the message queue to pop from.
             * @return The oldest message in the queue.
             */
            const Message PopMessage(const Wrapper::Socket::Protocol& protocol);

            /**
             * @brief Check if there are any messages in the player's message queue.
             *
             * @param protocol The protocol of the message queue to check.
             * @return True if there are at least one message, false otherwise.
             */
            bool HasMessage(const Wrapper::Socket::Protocol& protocol) const;

            /**
             * @brief Connect the player using provided credentials.
             *
             * @param username The username of the player.
             * @param password The password of the player.
             * @return True if the connection is successful, false otherwise.
             */
            bool Connect(const std::string& username, const std::string& password);

            /**
             * @brief Check if a player with the given username exists in the database.
             *
             * @param username The username to check.
             * @return True if the player exists, false otherwise.
             */
            bool DoesExist(const std::string& username) const;

            /**
             * @brief Register a new player with provided credentials.
             *
             * @param username The username of the player.
             * @param password The password of the player.
             * @return True if the registration is successful, false otherwise.
             */
            bool Register(const std::string& username, const std::string& password);

            /**
             * @brief Move the player in the specified direction.
             *
             * @param direction The direction to move the player.
             * @return The new position of the player.
             */
            const Position& Move(const Direction& direction);

            /**
             * @brief Check if the player is connected.
             *
             * @return True if the player is connected, false otherwise.
             */
            bool IsConnected() const;

            /**
             * @brief Get stored client address
             */
            const std::string& GetAddress() const;

            /**
             * @brief Get stored client port
             */
            std::uint16_t GetPort() const;

            /**
             * @brief Get unique identifier fetched from database
             *
             * @return Unique identifier
             */
            std::uint32_t GetId() const;

            /**
             * @brief Set the playing status of the player
             *
             * @param status True if the player is in a game session, false otherwise
             */
            void SetPlaying(bool status);

            /**
             * @brief Check if the player is currently in a game session
             *
             * @return True if the player is in a game session, false otherwise
             */
            bool IsPlaying() const;

            /**
             * @brief Set the alive status of the player
             *
             * @param status True if the player is alive, false otherwise
             */
            void SetAlive(bool status);

            /**
             * @brief Check if the player is currently alive in the game
             *
             * @return True if the player is alive, false otherwise
             */
            bool IsAlive() const;

            /**
             * @brief Set the position of the player
             *
             * @param position The new position of the player
             */
            void SetPosition(const Position& position);

            /**
             * @brief Update the player's position.
             *
             * @return The updated position of the player.
             */
            const Position& GetPosition() const;

            /**
             * @brief Get the role of the player
             *
             * @return The role of the player
             */
            const Role& GetRole() const;

            /**
             * @brief Set the shielded status of the player
             *
             * @param status True to activate shield, false to deactivate
             * @param god True to override existing status, false to respect current status
             */
            void SetStatistic(const Statistic& statistic, const bool status, const bool god = false);

            /**
             * @brief Check if a specific statistic is active for the player
             *
             * @param statistic The statistic to check
             * @return True if the statistic is active, false otherwise
             */
            bool IsStatisticActive(const Statistic& statistic) const;

            /**
             * @brief Check if a specific statistic has expired for the player
             *
             * @param statistic The statistic to check
             * @return True if the statistic has expired, false otherwise
             */
            bool IsStatisticGone(const Statistic& statistic) const;

        private:
            /**
             * @brief Convert a Statistic enum to its string representation
             *
             * @param statistic The statistic to convert
             * @return The string representation of the statistic
             */
            const std::string StatisticToString(const Statistic& statistic) const;

            /**
             * @brief Convert a Role enum to its string representation
             *
             * @param role The role to convert
             * @return The string representation of the role
             */
            const std::string RoleToString(const Role& role) const;

            mutable std::mutex _tcpMutex; /*!< Mutex to protect transmission control protocol message queue */
            mutable std::mutex _udpMutex; /*!< Mutex to protect user datagram protocol message queue */

            std::queue<Message> _tcp; /*!< The queue of messages to be sent to the player using transmission control protocol */
            std::queue<Message> _udp; /*!< The queue of messages to be sent to the player using user datagram protocol */
            std::string _username; /*!< The username of the player */
            std::string _address; /*!< The address where the player is connected */
            std::uint16_t _port; /*!< The port where the player is connected */
            std::uint32_t _id; /*!< The unique identifier of the player */

            std::unordered_map<Statistic, std::pair<Misc::Clock, bool>> _statistics; /*!< Clocks and statuses for various player statistics */
            Position _position; /*!< The current position of the player */
            Role _role; /*!< The role of the player fetched from database */
            bool _playing; /*!< Whether the player is currently in a game session */
            bool _alive; /*!< Whether the player is currently alive in the game */
            bool _god; /*!< Whether the player override the shield reset */
    };
}
