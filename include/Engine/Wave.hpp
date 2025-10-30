/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Wave.hpp
*/

#pragma once

#include <sol/forward.hpp>
#include <sol/sol.hpp>
#include <optional>
#include <cstdint>
#include <memory>

/**
 * @namespace Engine
 * @brief Contains classes and functions related to the game engine
 */
namespace Engine
{
    /**
     * @class Wave
     * @brief A class representing a wave in the game
     */
    class Wave
    {
        public:
            /**
             * @enum Result
             * @brief Possible results of wave processing
             */
            enum class Result : std::uint8_t {
                Continue = 0, /*!< The wave continues */
                Next = 1, /*!< Move to the next wave */
                Stop = 2 /*!< Stop the game */
            };

            /**
             * @brief Create a new wave for a specific game
             *
             * @param id The identifier of the game associated with this wave
             */
            explicit Wave(const std::uint32_t id);

            /**
             * @brief Destroy the wave
             */
            ~Wave();

            /**
             * @brief Process the wave logic
             *
             * @param deltaTime Time elapsed since last update in seconds
             * @return True if the game must stop, false otherwise
             */
            Result Process(float deltaTime);

        private:
            /**
            * @brief Get the path of the next wave script
            *
            * @return The file path of the next wave script
            */
            std::optional<std::string> GetNextWaveScriptPath() const;

            /**
             * @brief Register all Lua bindings for game control
             */
            void RegisterLuaBindings();

            /**
             * @brief Switch to the next wave script
             *
             * @return True if we have reached the end of the waves or an error occurred, false otherwise
             */
            Result SwitchToNextWave();

            /**
             * @brief Load the wave script from the specified file path
             *
             * @param filepath The path to the wave script file
             * @throw Exception if the file cannot be loaded
             */
            void LoadWaveScript(const std::string& filepath);

            /**
             * @brief Callback to load the wave script
             *
             * @throw Exception if the wave script does not contain the required process function
             */
            void LoadWaveCallback();

            /**
             * @brief Initialize the wave by loading the Lua script
             *
             * @throw Exception if the OnInit function fails
             */
            void InitializeWave();

            sol::state _state; /*!< The Lua state for wave scripting */
            std::weak_ptr<void> _game; /*!< The game associated with this wave (weak reference to avoid circular dependency) */
            sol::function _callback; /*!< The Lua function to call for wave processing */
            std::uint8_t _next; /*!< The current wave index */
            std::uint32_t _id; /*!< The unique identifier of the wave's game */
    };
}
