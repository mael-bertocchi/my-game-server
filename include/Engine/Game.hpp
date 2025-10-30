/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Game.hpp
*/

#pragma once

#include "Miscellaneous/Clock.hpp"
#include "Engine/Collision.hpp"
#include "Network/Player.hpp"
#include "Engine/Wave.hpp"
#include "Types.hpp"

#include <unordered_map>
#include <sol/sol.hpp>
#include <functional>
#include <optional>
#include <cstdint>
#include <vector>

/**
 * @namespace Engine
 * @brief Contains classes and functions related to the game engine
 */
namespace Engine
{
    /**
     * @class Game
     * @brief A class representing a game
     */
    class Game
    {
        public:
            /**
             * @brief Create a new game
             */
            explicit Game();

            /**
             * @brief Destroy the game
             */
            ~Game();

            /**
             * @brief Get the unique identifier of the game
             *
             * @return The unique identifier of the game
             */
            std::uint32_t GetId() const;

            /**
             * @brief Add a player to the game
             *
             * @param id The unique identifier of the player to add
             * @return True if the player was added successfully, false if the player is already in the game
             */
            bool AddPlayerId(const std::uint32_t id);

            /**
             * @brief Remove a player from the game
             *
             * @param id The unique identifier of the player to remove
             */
            bool RemovePlayerId(const std::uint32_t id);

            /**
             * @brief Kill a player in the game
             *
             * @param player The identifier of the player to kill
             */
            bool KillPlayerId(const std::uint32_t id);

            /**
             * @brief Get the list of player identifiers in the game
             *
             * @return An array of player identifiers
             */
            std::array<std::uint32_t, MAX_PLAYER_PER_GAMES> GetPlayerIds() const;

            /**
             * @brief Get the current number of players in the game
             *
             * @return The number of players currently in the game
             */
            std::uint8_t GetPlayerCount() const;

            /**
             * @brief Get the map of missiles in the game
             *
             * @param type The type of missiles to retrieve
             * @return A reference to the unordered_map of missiles
             */
            std::unordered_map<std::uint32_t, Entity>& GetMissiles(const MissileType type);

            /**
             * @brief Get a missile by its identifier and type
             *
             * @param id The unique identifier of the missile to retrieve
             * @param type The type of missile
             * @return A reference wrapper to the missile if found, nullopt otherwise
             */
            std::optional<std::reference_wrapper<Entity>> GetMissile(const std::uint32_t id, const MissileType type);

            /**
             * @brief Create a new player missile in the game
             *
             * @param position The initial position of the player missile
             * @param type The type of missile
             * @return The unique identifier of the created player missile
             */
            std::uint32_t CreateMissile(Position position, const MissileType type);

            /**
             * @brief Move enemy missile in the game
             *
             * @param id The unique identifier of the missile to move
             * @param dx The change in X position
             * @param dy The change in Y position
             */
            void MoveMissile(std::uint32_t id, const std::int16_t dx, const std::int16_t dy);

            /**
             * @brief Move player missile in the game
             *
             * @param missile The missile to move
             * @param type The type of missile being moved
             * @return True if the missile should be removed (out of bounds), false otherwise
             */
            bool MoveMissile(Entity& missile, const MissileType type);

            /**
             * @brief Remove missile from the game
             *
             * @param id The unique identifier of the missile to remove
             * @param type The type of missile
             */
            void RemoveMissile(const std::uint32_t id, const MissileType type);

            /**
             * @brief Check if the game has started
             *
             * @return True if the game has started, false otherwise
             */
            bool IsStarted() const;

            /**
             * @brief Check if the game is inactive for too long
             *
             * @return True if the game is inactive, false otherwise
             */
            bool IsInactive();

            /**
             * @brief Process the game logic for the
             */
            void Process();

            /**
             * @brief Start the game
             */
            void Start();

            /**
             * @brief Stop the game
             */
            void Stop();

            /**
             * @brief Move to the next wave in the game
             */
            void Next();

            /**
             * @brief Queue a position update to be sent later in batch
             *
             * @param entity The type of entity
             * @param id The unique identifier of the entity
             * @param position The new position of the entity
             */
            void QueuePosition(const std::uint32_t id, const std::uint8_t entity, const Position position);

            /**
             * @brief Get all enemies of a specific type
             *
             * @param type The type of enemies to retrieve
             * @return A reference to the unordered_map of enemies
             */
            std::unordered_map<std::uint32_t, Entity>& GetEnemies(const EnemyType type);

            /**
             * @brief Get an enemy by its identifier and type
             *
             * @param id The unique identifier of the enemy to retrieve
             * @param type The type of enemy
             * @return A reference wrapper to the enemy if found, nullopt otherwise
             */
            std::optional<std::reference_wrapper<Entity>> GetEnemy(const std::uint32_t id, const EnemyType type);

            /**
             * @brief Create a new enemy in the game at a specific position
             *
             * @param position The position where the enemy should spawn
             * @param type The type of enemy to create
             * @return The unique identifier of the created enemy
             */
            std::uint32_t CreateEnemy(const Position position, const EnemyType type);

            /**
             * @brief Move an enemy by its identifier
             *
             * @param id The unique identifier of the enemy to move
             * @param type The type of enemy to move
             * @param dx The change in X position
             * @param dy The change in Y position
             */
            void MoveEnemy(const std::uint32_t id, const EnemyType type, const std::int16_t dx, const std::int16_t dy);

            /**
             * @brief Remove an enemy from the game
             *
             * @param id The unique identifier of the enemy to remove
             * @param type The type of enemy to remove
             */
            void RemoveEnemy(const std::uint32_t id, const EnemyType type);

            /**
             * @brief Damage an enemy and remove it if health drops to or below 0
             *
             * @param id The unique identifier of the enemy to damage
             * @param type The type of enemy to damage
             * @param damage The amount of damage to deal
             * @return True if the enemy was killed, false if still alive
             */
            bool DamageEnemy(const std::uint32_t id, const EnemyType type, const std::int32_t damage);

            /**
             * @brief Get all items of a specific type
             *
             * @param type The type of items to retrieve
             * @return A reference to the unordered_map of items
             */
            std::unordered_map<std::uint32_t, Entity>& GetItems(const ItemType type);

            /**
             * @brief Get an item by its identifier and type
             *
             * @param id The unique identifier of the item to retrieve
             * @param type The type of item
             * @return A reference wrapper to the item if found, nullopt otherwise
             */
            std::optional<std::reference_wrapper<Entity>> GetItem(const std::uint32_t id, const ItemType type);

            /**
             * @brief Create a new item in the game at a specific position
             *
             * @param position The position where the item should spawn
             * @param type The type of item to create
             * @return The unique identifier of the created item
             */
            std::uint32_t CreateItem(const Position position, const ItemType type);

            /**
             * @brief Remove an item from the game
             *
             * @param id The unique identifier of the item to remove
             * @param type The type of item to remove
             */
            void RemoveItem(const std::uint32_t id, const ItemType type);

        private:
            /**
             * @enum TimedEvent
             * @brief The types of timed events in the game
             */
            enum class TimedEvent: std::uint8_t {
                Inactivity = 0, /*!< Clock for checking inactivity */
                Shield = 1, /*!< Clock for player shields */
                Wave = 2, /*!< Clock for wave processing */
                Move = 3 /*!< Clock for moving entities */
            };

            /**
             * @brief Broadcast a player's statistic change to all players in the game
             *
             * @param id The unique identifier of the player
             * @param statistic The statistic that changed
             * @param status The new status of the statistic
             */
            void SetPlayerIdStatistic(const std::uint32_t id, const Network::Player::Statistic& statistic, const bool status);

            /**
             * @brief Broadcast a player's statistic change to all players in the game
             *
             * @param player The player whose statistic changed
             * @param statistic The statistic that changed
             * @param status The new status of the statistic
             */
            void SetPlayerIdStatistic(const std::shared_ptr<Network::Player>& player, const Network::Player::Statistic& statistic, const bool status);

            /**
             * @brief Check if a player is in the game
             *
             * @param id The unique identifier of the player
             * @return The slot index if found, -1 otherwise
             */
            std::int8_t GetPlayerIdSlot(const std::uint32_t id) const;

            /**
             * @brief Get the next available player slot in the game
             *
             * @param id The unique identifier of the player to check
             * @return The index of the next available player slot, or -1 if the game is full or player already in game
             */
            std::int8_t GetNextAvailablePlayerIdSlot(const std::uint32_t id) const;

            /**
             * @brief Apply collision results by removing/killing entities
             *
             * @param result The collision result containing entities to delete
             */
            void ApplyCollisions(const Collision::Result& result);

            /**
             * @brief Process internal game logic such as moving missiles and checking collisions
             */
            void MoveEntities();

            /**
             * @brief Send all queued position updates to all players in the game
             */
            void SendPosition();

            std::vector<std::tuple<std::uint32_t, std::uint8_t, Position>> _positions; /*!< Queue of position updates to be sent in batch */
            std::array<std::uint32_t, MAX_PLAYER_PER_GAMES> _ids; /*!< Array of player identigiers */
            std::unordered_map<TimedEvent, Misc::Clock> _clocks; /*!< Map of clocks for timing events */
            std::unique_ptr<Wave> _wave; /*!< Unique pointer to the current wave */
            std::uint32_t _id; /*!< Unique identifier for the game */

            Missiles _missiles; /*!< Structure holding player and enemy missiles */
            Enemies _enemies; /*!< Structure holding different types of enemies */
            Items _items; /*!< Structure holding different types of items */
            bool _inactive; /*!< Whether the game is checking for inactivity */
            bool _started; /*!< Whether the game has started or not */
    };
}
