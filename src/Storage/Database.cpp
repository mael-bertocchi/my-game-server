/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Database.cpp
*/

#include "Miscellaneous/Environment.hpp"
#include "Miscellaneous/Logger.hpp"
#include "Exception/Generic.hpp"
#include "Storage/Database.hpp"

#include <format>

void Storage::Database::Connect()
{
    if (IsConnected()) {
        throw Exception::GenericError("Database is already connected");
    }
    auto& env = Misc::Env::GetInstance().GetConfiguration<Misc::Env::Database>();

    _connexion = std::make_unique<pqxx::connection>(std::format("host={} port={} dbname={} user={} password={}", env.host, env.port, env.dbname, env.user, env.password));

    if (IsConnected()) {
        Misc::Logger::Log(std::format("Connected to database {} as {}", env.dbname, env.user));
    } else {
        throw Exception::GenericError("Failed to connect to the database");
    }
}

void Storage::Database::Disconnect()
{
    if (IsConnected()) {
        _connexion->close();
        _connexion.reset();
        Misc::Logger::Log("Disconnected from database");
    } else {
        throw Exception::GenericError("Database is not connected");
    }
}

bool Storage::Database::IsConnected() const
{
    return _connexion && _connexion->is_open();
}

const std::unique_ptr<pqxx::connection>& Storage::Database::GetConnection() const
{
    if (IsConnected()){
        return _connexion;
    }
    throw Exception::GenericError("Database connection is not established");
}

const std::unique_ptr<pqxx::work> Storage::Database::CreateTransaction() const
{
    if (IsConnected()){
        return std::make_unique<pqxx::work>(*_connexion);
    }
    throw Exception::GenericError("Database connection is not established");
}
