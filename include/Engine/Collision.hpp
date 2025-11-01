/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Collision.hpp
*/

#pragma once

#include "Variables.hpp"
#include "Types.hpp"

#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cstdint>
#include <vector>
#include <array>

/**
 * @namespace Engine
 * @brief Contains classes and functions related to the game engine
 */
namespace Engine
{
    /**
     * @class Collision
     * @brief A class responsible for handling collision detection and response.
     */
    class Collision
    {
        public:
            /**
             * @struct MissilesResult
             * @brief Structure containing missile collision results
             */
            struct MissilesResult {
                std::unordered_set<std::uint32_t> player; /*!< Player missiles to delete */
                std::unordered_set<std::uint32_t> enemy; /*!< Enemy missiles to delete */
                std::unordered_set<std::uint32_t> force; /*!< Player force missiles to delete */
                std::unordered_set<std::uint32_t> boss; /*!< Boss missiles to delete */
            };

            /**
             * @struct EnemiesResult
             * @brief Structure containing enemy collision results
             */
            struct EnemiesResult {
                std::unordered_set<std::uint32_t> generic; /*!< Standard enemies to delete */
                std::unordered_set<std::uint32_t> walking; /*!< Walking enemies to delete */
                std::unordered_set<std::uint32_t> flying; /*!< Flying enemies to delete */
                std::unordered_set<std::uint32_t> boss; /*!< Boss enemies to delete */
            };

            /**
             * @struct DamagedEnemiesResult
             * @brief Structure containing damaged enemy collision results (not dead)
             */
            struct DamagedResult {
                std::unordered_map<std::uint32_t, std::int32_t> generic; /*!< Generic enemies damaged */
                std::unordered_map<std::uint32_t, std::int32_t> walking; /*!< Walking enemies damaged */
                std::unordered_map<std::uint32_t, std::int32_t> flying; /*!< Flying enemies damaged */
                std::unordered_map<std::uint32_t, std::int32_t> boss; /*!< Boss enemies damaged */
            };

            /**
             * @struct CollisionResult
             * @brief Structure containing collision results with entities to be deleted
             */
            struct Result {
                std::unordered_map<std::uint32_t, std::uint32_t> shields; /*!< Map of shield item identifiers to player identifiers who collected them */
                std::unordered_map<std::uint32_t, std::uint32_t> forces; /*!< Map of force item identifiers to player identifiers who collected them */
                std::vector<std::uint32_t> players; /*!< identifiers of players to kill */
                MissilesResult missiles; /*!< Missiles to delete */
                EnemiesResult enemies; /*!< Enemies to delete by type */
                DamagedResult damaged; /*!< Enemies damaged but not dead */
            };

            /**
             * @brief Check all collisions in the game and return entities to delete
             *
             * @param ids Array of player identifiers
             * @param enemies Structure containing all enemy types
             * @param missiles Structure containing all missile types
             * @param items Structure containing all item types
             * @return The ids of entities to delete, organized by type
             */
            static Result Check(const std::array<std::uint32_t, MAX_PLAYER_PER_GAMES>& ids, const Enemies& enemies, const Missiles& missiles, const Items& items);

        private:
            /**
             * @struct BoundingBox
             * @brief Axis-Aligned Bounding Box for collision detection
             */
            struct BoundingBox {
                std::uint16_t height; /*!< Height of the box */
                std::uint16_t width; /*!< Width of the box */
                std::uint16_t x; /*!< Top-left X position */
                std::uint16_t y; /*!< Top-left Y position */
            };

            /**
             * @brief Check for AABB (Axis-Aligned Bounding Box) collision between two boxes
             *
             * @param first First bounding box
             * @param second Second bounding box
             * @return True if the boxes collide, false otherwise
             */
            static bool DoesCollide(const BoundingBox& first, const BoundingBox& second);

            /**
             * @brief Create bounding box from position and dimensions
             *
             * @param position The position of the entity
             * @param width The width of the entity
             * @param height The height of the entity
             * @return A bounding box for the entity
             */
            static BoundingBox CreateBoundingBox(const Position& position, const std::uint16_t width, const std::uint16_t height);
    };
}
