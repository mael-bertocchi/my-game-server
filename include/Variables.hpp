/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Variables.hpp
*/

#pragma once

#include <cstdint>

/*!< Related to server configuration */

constexpr std::uint32_t CRYPTION_ITERATIONS = 10000; /*!< Number of iterations for password hashing */

constexpr std::uint8_t CRYPTION_HASH_LENGTH = 32; /*!< Length of the hashed password */

constexpr std::uint8_t CRYPTION_SALT_LENGTH = 16; /*!< Length of the salt used in password hashing */

constexpr std::uint8_t SLEEP_DURATION_MS = 16; /*!< Duration to sleep in the main loop */

/*!< Related to networking */

constexpr std::uint32_t MAX_TCP_MESSAGE_SIZE = 1024 * 1024 * 50; /*!< Maximum size for messages (50MB) */

constexpr std::uint16_t UDP_PACKET_SIZE = 2048; /*!< Size of each udp packet */

constexpr std::uint8_t HEADER_FRAGMENTS_SIZE = 2; /*!< Size of the fragments field in bytes */

constexpr std::uint8_t HEADER_LENGTH_SIZE = 4; /*!< Size of the message length field in bytes */

constexpr std::uint8_t HEADER_TYPE_SIZE = 1; /*!< Size of the message type field in bytes */

constexpr std::uint8_t HEADER_ID_SIZE = 4; /*!< Size of the message identifier field in bytes */

constexpr std::uint8_t POLL_TIMEOUT_MS = 100; /*!< Timeout for poll in milliseconds */

constexpr std::uint8_t TCP_HEADER_SIZE = HEADER_TYPE_SIZE + HEADER_LENGTH_SIZE; /*!< Size of the message header (Type + Length) */

/*!< Related to the game logic */

constexpr std::uint32_t PLAYER_STATISTIC_DURATION_MS = 30000; /*!< Interval between entity movements */

constexpr std::uint32_t GAME_INACTIVITY_TIMEOUT_MS = 25000; /*!< Time in milliseconds before a game with no players is closed */

constexpr std::uint8_t GAME_PROCESS_INTERVAL_MS = 10; /*!< Interval between entity movements */

constexpr std::uint8_t ENTITY_MOVE_INTERVAL_MS = 100; /*!< Interval between entity movements */

constexpr std::uint8_t MAX_SPAWNABLE_ENTITY_VALUE = 9; /*!< Maximum value for spawnable entity types */

constexpr std::uint8_t MAX_ENTITY_VALUE = 10; /*!< Maximum value for entity types */

constexpr std::uint8_t MAX_PLAYER_PER_GAMES = 4; /*!< Maximum number of players allowed in a single game */

constexpr std::uint8_t MIN_PLAYER_PER_GAMES = 2; /*!< Minimum number of players required to start a game */

constexpr std::uint16_t WINDOW_HEIGHT = 600; /*!< Height of the game area */

constexpr std::uint16_t WINDOW_WIDTH = 900; /*!< Width of the game area */

/*!< Entity configuration */

constexpr std::uint16_t BOSS_ENEMY_HEALTH = 750; /*!< Health of boss enemies */

constexpr std::uint8_t GENERIC_ENEMY_HEALTH = 80; /*!< Health of generic enemies */

constexpr std::uint8_t WALKING_ENEMY_HEALTH = 120; /*!< Health of walking enemies */

constexpr std::uint8_t FLYING_ENEMY_HEALTH = 40; /*!< Health of flying enemies */

constexpr std::uint8_t PLAYER_MISSILE_DAMAGE = 20; /*!< Damage dealt by player missiles */

constexpr std::uint8_t FORCE_MISSILE_DAMAGE = 40; /*!< Damage dealt by player missiles */

constexpr std::uint8_t MISSILE_MOVE_SPEED = 40; /*!< Speed at which bullets move */

constexpr std::uint8_t PLAYER_MOVE_SPEED = 4; /*!< Speed at which players move */
