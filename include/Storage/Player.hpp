/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Player.hpp
*/

#pragma once

#include "Miscellaneous/Singleton.hpp"
#include "Network/Player.hpp"
#include "Wrapper/Socket.hpp"

#include <unordered_map>
#include <shared_mutex>
#include <optional>
#include <cstdint>
#include <memory>
#include <string>

/**
 * @namespace Storage
 * @brief Contains storage-related classes and functions
 */
namespace Storage::Cache
{
    /**
     * @class Player
     * @brief Caches connected players for quick access
     */
    class Player : public Misc::Singleton<Player>
    {
        public:
            /**
             * @typedef AddressToPlayerMap
             * @brief Type alias for the map of network addresses to player shared pointers for fast UDP lookup
             */
            using AddressToPlayerMap = std::unordered_map<std::string, std::shared_ptr<Network::Player>>;

            /**
             * @typedef SocketToPlayerMap
             * @brief Type alias for the map of socket descriptors to player shared pointers
             */
            using SocketToPlayerMap = std::unordered_map<Wrapper::Socket::SocketType, std::shared_ptr<Network::Player>>;

            /**
             * @typedef IdToPlayerMap
             * @brief Type alias for the map of session identifiers to player shared pointers
             */
            using IdToPlayerMap = std::unordered_map<std::uint32_t, std::shared_ptr<Network::Player>>;

            /**
             * @typedef IdToSocketMap
             * @brief Type alias for the map of player identifiers to socket descriptors for fast reverse lookup
             */
            using IdToSocketMap = std::unordered_map<std::uint32_t, Wrapper::Socket::SocketType>;

            /**
             * @brief Get all the connected players by socket
             *
             * @return A constant reference to the unordered map of socket descriptors to player shared pointers
             */
            const SocketToPlayerMap& GetSocketToPlayerMap() const;

            /**
             * @brief Get all the connected players by identifier
             *
             * @return A constant reference to the unordered map of session identifiers to player shared pointers
             */
            const IdToPlayerMap& GetIdToPlayerMap() const;

            /**
             * @brief Get a player by their socket descriptor
             *
             * @param socket The socket descriptor of the player to retrieve
             * @return A shared pointer to the player if found, nullptr otherwise
             */
            const std::shared_ptr<Network::Player> GetPlayerBySocket(const Wrapper::Socket::SocketType socket) const;

            /**
             * @brief Get a player by their session identifier
             *
             * @param id The session identifier of the player to retrieve
             * @return A shared pointer to the player if found, nullptr otherwise
             */
            const std::shared_ptr<Network::Player> GetPlayerById(const std::uint32_t id) const;

            /**
             * @brief Get a player by their network address and port
             *
             * @param address The IP address of the player to retrieve
             * @param port The port of the player to retrieve
             * @return A shared pointer to the player if found, nullptr otherwise
             */
            const std::shared_ptr<Network::Player> GetPlayerByAddress(const std::string& address, const std::uint16_t port) const;

            /**
             * @brief Get the socket associated with a session identifier
             *
             * @param id The session identifier to look up
             * @return The socket descriptor if found, std::nullopt otherwise
             */
            std::optional<Wrapper::Socket::SocketType> GetSocketByPlayerId(const std::uint32_t id) const;

            /**
             * @brief Get the session identifier associated with a socket
             *
             * @param socket The socket descriptor to look up
             * @return The session identifier if found, std::nullopt otherwise
             */
            std::optional<std::uint32_t> GetPlayerIdBySocket(const Wrapper::Socket::SocketType socket) const;

            /**
             * @brief Add a new player to the cache
             *
             * @param socket The socket descriptor of the player to add
             * @param player A shared pointer to the player to add
             */
            void AddPlayer(const Wrapper::Socket::SocketType socket, std::shared_ptr<Network::Player> player);

            /**
             * @brief Remove a player from the cache by their socket descriptor
             *
             * @param socket The socket descriptor of the player to remove
             * @return The session identifier of the removed player
             */
            std::uint32_t RemovePlayer(const Wrapper::Socket::SocketType socket);

        private:
            /**
             * @brief Allow Singleton to access the private constructor and destructor
             */
            friend class Misc::Singleton<Player>;

            /**
             * @brief Default constructor for the Player class to prevent direct instantiation
             */
            Player() = default;

            /**
             * @brief Default destructor for the Player class to prevent direct destruction
             */
            ~Player() = default;

            AddressToPlayerMap _addressToPlayer; /*!< Map of connected players by network address for fast udp lookup */
            SocketToPlayerMap _socketToPlayer; /*!< Map of connected players by socket */
            IdToPlayerMap _idToPlayer; /*!< Map of connected players by session identifier */
            IdToSocketMap _idToSocket; /*!< Map of player identifiers to socket descriptors for fast reverse lookup */

            mutable std::shared_mutex _mutex; /*!< Shared mutex for thread-safe player access */
    };
}
