/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Types.hpp
*/

#pragma once

#include "Miscellaneous/Maths.hpp"

#include <unordered_map>
#include <cstdint>

/**
 * @enum ActionType
 * @brief The different types of actions in the game.
 */
enum class ActionType : std::uint8_t {
    UKN = 0, /*!< Unknown message type */
    ERR = 1, /*!< An error occurred */
    DFY = 2, /*!< Identify message type */
    OVE = 3, /*!< The player moved */
    POS = 4, /*!< The position of the player */
    JON = 5, /*!< The player joined a game */
    LVE = 6, /*!< The player left a game */
    SPW = 7, /*!< Spawn an entity */
    SHT = 8, /*!< Shoot a bullet */
    DIE = 9, /*!< An entity died */
    CRE = 10, /*!< Create a game */
    STR = 11, /*!< Start the game */
    STP = 12, /*!< Stop the game */
    STS = 13, /*!< Shield status update */
    NXT = 14, /*!< Next wave notification */
    GOD = 15 /*!< Toggle god mode */
};

/**
 * @typedef Position
 * @brief A 2D position represented by a vector of unsigned 16-bit integers.
 */
using Position = Misc::Maths::Vector2<std::uint16_t>;

/**
 * @enum Statistic
 * @brief The different types of statistics.
 */
enum class Statistic : std::uint8_t {
    Shield = 0, /*!< The shield statistic */
    Force = 1 /*!< The force statistic for more powerful missiles */
};

/**
 * @enum Role
 * @brief The different roles a player can have.
 */
enum class Role : std::uint8_t {
    Administrator = 0, /*!< Administrator with elevated privileges */
    Player = 1 /*!< Regular player */
};

/**
 * @enum Direction
 * @brief The possible movement directions for the player.
 */
enum class Direction : std::uint8_t {
    DownRight = 0, /*!< Move down and to the right */
    DownLeft = 1, /*!< Move down and to the left */
    UpRight = 2, /*!< Move up and to the right */
    UpLeft = 3, /*!< Move up and to the left */
    Right = 4, /*!< Move right */
    Down = 5, /*!< Move down */
    Left = 6, /*!< Move left */
    Up = 7 /*!< Move up */
};

/**
 * @enum MissileType
 * @brief The different types of missiles in the game.
 */
enum class Missile : std::uint8_t {
    Player = 0, /*!< Player missile */
    Enemy = 1, /*!< Enemy missile */
    Force = 2, /*!< Player force missile */
    Boss = 3 /*!< Boss missile */
};

/**
 * @enum EnemyType
 * @brief The different types of enemies in the game.
 */
enum class Enemy : std::uint8_t {
    Generic = 4, /*!< The default enemy */
    Walking = 5, /*!< An enemy that walks on the ground */
    Flying = 6, /*!< An enemy that flies on the air */
    Boss = 7 /*!< A powerful boss enemy */
};

/**
 * @enum ItemType
 * @brief The different types of items in the game.
 */
enum class Item : std::uint8_t {
    Shield = 8, /*!< Shield item */
    Force = 9 /*!< Force item */
};

/**
 * @enum CharacterType
 * @brief The different types of characters in the game.
 */
enum class Character : std::uint8_t {
    Player = 10 /*!< Player character */
};

/**
 * @struct Entity
 * @brief Structure representing a generic entity in the game.
 */
struct Entity {
    Position position; /*!< The position of the entity */
    std::uint32_t id; /*!< The unique identifier of the entity */
    std::int32_t health; /*!< The health points of the entity (default 100) */
};

/**
 * @struct Missiles
 * @brief Structure holding player and enemy missiles
 */
struct Missiles {
    std::unordered_map<std::uint32_t, Entity> player; /*!< Player missile */
    std::unordered_map<std::uint32_t, Entity> enemy; /*!< Enemy missile */
    std::unordered_map<std::uint32_t, Entity> force; /*!< Player force missile */
    std::unordered_map<std::uint32_t, Entity> boss; /*!< Boss missile */
};

/**
 * @struct Enemies
 * @brief Structure holding different types of enemies
 */
struct Enemies {
    std::unordered_map<std::uint32_t, Entity> generic; /*!< Generic enemy */
    std::unordered_map<std::uint32_t, Entity> walking; /*!< Enemy that walks on the ground */
    std::unordered_map<std::uint32_t, Entity> flying; /*!< Enemy that flies */
    std::unordered_map<std::uint32_t, Entity> boss; /*!< Boss enemy */
};

/**
 * @struct Items
 * @brief Structure holding different types of items
 */
struct Items {
    std::unordered_map<std::uint32_t, Entity> shield; /*!< Shield items */
    std::unordered_map<std::uint32_t, Entity> force; /*!< Force items */
};
