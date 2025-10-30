/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Environment.hpp
*/

#pragma once

#include "Miscellaneous/Singleton.hpp"

#include <libconfig.h++>
#include <shared_mutex>
#include <cstdint>
#include <string>
#include <vector>
#include <mutex>

/**
 * @namespace Misc
 * @brief Contains miscellaneous utility classes and functions.
 */
namespace Misc
{
    /**
     * @class Environment
     * @brief Singleton class to manage environment configurations.
     */
    class Env : public Singleton<Env>
    {
        public:
            /**
             * @struct Database
             * @brief Structure to hold database configuration parameters.
             */
            struct Database {
                std::string host; /*!< The host address of the database server */
                std::uint16_t port; /*!< The port number of the database server */
                std::string dbname; /*!< The name of the database */
                std::string user; /*!< The username for database authentication */
                std::string password; /*!< The password for database authentication */
            };

            /**
             * @struct Server
             * @brief Structure to hold server configuration parameters.
             */
            struct Server {
                std::uint16_t port; /*!< The port number of the server */
            };

            /**
             * @struct Waves
             * @brief Structure to hold wave configuration parameters.
             */
            struct Waves {
                std::vector<std::string> files; /*!< List of wave file paths */
            };

            /**
             * @brief Load environment configurations from a file.
             *
             * @param filepath The path to the configuration file.
             * @throw Exception if the file cannot be read or parsed.
             */
            void LoadFromFile(const std::string& filepath);

            /**
             * @brief Get the configuration for a specific category.
             *
             * @param category The configuration category to retrieve.
             * @return The configuration object for the specified category.
             */
            template<typename T>
            const T& GetConfiguration() const;

        private:
            /**
             * @brief Allow Singleton to access the private constructor and destructor.
             */
            friend class Singleton<Env>;

            /**
             * @brief Default constructor for the Environment class to prevent direct instantiation.
             */
            Env();

            /**
             * @brief Default destructor for the Environment class to prevent direct destruction.
             */
            ~Env() = default;

            /**
             * @brief Load database configuration from a libconfig setting.
             *
             * @param db The libconfig setting containing database configuration.
             * @throw Exception if required parameters are missing or invalid.
             */
            void LoadDatabaseFromFile(const libconfig::Setting& db);

            /**
             * @brief Load server configuration from a libconfig setting.
             *
             * @param server The libconfig setting containing server configuration.
             * @throw Exception if required parameters are missing or invalid.
             */
            void LoadServerFromFile(const libconfig::Setting& server);

            /**
             * @brief Load waves configuration from a libconfig setting.
             *
             * @param waves The libconfig setting containing waves configuration.
             * @throw Exception if required parameters are missing or invalid.
             */
            void LoadWavesFromFile(const libconfig::Setting& waves);

            mutable std::shared_mutex _mutex; /*!< Shared mutex for thread-safe configuration access */
            Database _database; /*!< The database configuration parameters. */
            Server _server; /*!< The server configuration parameters. */
            Waves _waves; /*!< The waves configuration parameters. */
            bool _loaded; /*!< Flag to indicate if the configurations have been loaded. */
    };
}
