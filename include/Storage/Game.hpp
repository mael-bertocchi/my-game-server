/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Game.hpp
*/

#pragma once

#include "Miscellaneous/Singleton.hpp"
#include "Engine/Game.hpp"

#include <unordered_map>
#include <shared_mutex>
#include <cstdint>
#include <memory>
#include <vector>
#include <mutex>

/**
 * @namespace Storage
 * @brief Contains storage-related classes and functions
 */
namespace Storage::Cache
{
    /**
     * @class Game
     * @brief Caches game sessions for quick access
     */
    class Game : public Misc::Singleton<Game>
    {
        public:
            /**
             * @typedef IdToGameMap
             * @brief Type alias for the map of session identifiers to game shared pointers
             */
            using IdToGameMap = std::unordered_map<std::uint32_t, std::shared_ptr<Engine::Game>>;

            /**
             * @typedef PlayerToGameMap
             * @brief Type alias for the map of player identifiers to game identifiers for fast lookup
             */
            using PlayerToGameMap = std::unordered_map<std::uint32_t, std::uint32_t>;

            /**
             * @brief Get all the game sessions
             */
            const IdToGameMap& GetIdToGameMap() const;

            /**
             * @brief Get all game identifiers without holding shared_ptr references
             *
             * @return A vector of game identifiers
             */
            std::vector<std::uint32_t> GetGameIds() const;

            /**
             * @brief Add a new game session to the cache
             *
             * @param game A shared pointer to the game session to add
             */
            void AddGame(std::shared_ptr<Engine::Game> game);

            /**
             * @brief Get a game session by its unique identifier
             *
             * @param id The unique identifier of the game session
             * @return A shared pointer to the game session if found, nullptr otherwise
             */
            std::shared_ptr<Engine::Game> GetGameById(const std::uint32_t id) const;

            /**
             * @brief Get a game session by a player's unique identifier
             *
             * @param id The unique identifier of the player
             * @return A shared pointer to the game session if found, nullptr otherwise
             */
            std::shared_ptr<Engine::Game> GetGameByPlayerId(const std::uint32_t id) const;

            /**
             * @brief Add a player to game mapping
             *
             * @param playerId The unique identifier of the player
             * @param gameId The unique identifier of the game
             */
            void AddPlayerToGame(const std::uint32_t playerId, const std::uint32_t gameId);

            /**
             * @brief Remove a player from game mapping
             *
             * @param id The unique identifier of the player
             */
            void RemovePlayerFromGame(const std::uint32_t id);

            /**
             * @brief Remove a game session from the cache by its unique identifier
             *
             * @param id The unique identifier of the game session to remove
             */
            void RemoveGame(const std::uint32_t id);

        private:
            /**
             * @brief Allow Singleton to access the private constructor and destructor
             */
            friend class Misc::Singleton<Game>;

            /**
             * @brief Default constructor for the Game class to prevent direct instantiation
             */
            Game() = default;

            /**
             * @brief Default destructor for the Game class to prevent direct destruction
             */
            ~Game() = default;

            PlayerToGameMap _playerToGame; /*!< Map of player identifiers to game identifiers */
            IdToGameMap _idToGame; /*!< Map of game sessions by session identifier */

            mutable std::shared_mutex _mutex; /*!< Shared mutex for thread-safe game access */
    };
}
