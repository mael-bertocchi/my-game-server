/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Database.hpp
*/

#pragma once

#include "Miscellaneous/Singleton.hpp"

#include <pqxx/pqxx>
#include <memory>

namespace Storage
{
    class Database : public Misc::Singleton<Database>
    {
        public:
            /**
             * @brief Connect to the database.
             *
             * @throw Exception if the connection fails.
             */
            void Connect();

            /**
             * @brief Disconnect from the database.
             */
            void Disconnect();

            /**
             * @brief Check if the database connection is active.
             *
             * @return true if the database is connected, false otherwise.
             */
            bool IsConnected() const;

            /**
             * @brief Get the current database connection.
             *
             * @return A shared pointer to the pqxx::connection object.
             */
            const std::unique_ptr<pqxx::connection>& GetConnection() const;

            /**
             * @brief Create a new transaction on the current database connection.
             *
             * @return A unique pointer to the pqxx::work object representing the transaction.
             */
            const std::unique_ptr<pqxx::work> CreateTransaction() const;

        private:
            /**
             * @brief Allow Singleton to access the private constructor and destructor.
             */
            friend class Misc::Singleton<Database>;

            /**
             * @brief Default constructor for the Database class to prevent direct instantiation.
             */
            Database() = default;

            /**
             * @brief Default destructor for the Database class to prevent direct destruction.
             */
            ~Database() = default;

            std::unique_ptr<pqxx::connection> _connexion; /*!< The database connection object. */
    };
}
