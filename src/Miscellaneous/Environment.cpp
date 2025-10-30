/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Environment.cpp
*/

#include "Miscellaneous/Environment.hpp"
#include "Miscellaneous/Logger.hpp"
#include "Exception/Generic.hpp"

#include <shared_mutex>
#include <filesystem>
#include <format>

Misc::Env::Env() : _loaded(false) {}

void Misc::Env::LoadFromFile(const std::string& filepath)
{
    std::unique_lock<std::shared_mutex> lock(_mutex);

    if (_loaded) {
        throw Exception::GenericError("Environment configuration is already loaded");
    }

    libconfig::Config cfg;

    try {
        cfg.readFile(filepath.c_str());

        const libconfig::Setting& root = cfg.getRoot();

        LoadDatabaseFromFile(root["database"]);
        LoadServerFromFile(root["server"]);
        LoadWavesFromFile(root["waves"]);
    } catch (const libconfig::FileIOException&) {
        throw Exception::GenericError(std::format("I/O error while reading file: {}", filepath));
    } catch (const libconfig::ParseException &pex) {
        throw Exception::GenericError(std::format("Parse error at {}:{} - {}", pex.getFile(), pex.getLine(), pex.getError()));
    }

    Misc::Logger::Log("Environment configuration loaded");

    _loaded = true;
}

template <typename T>
const T& Misc::Env::GetConfiguration() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);

    if (!_loaded) {
        throw Exception::GenericError("Environment configurations not loaded.");
    }
    if constexpr (std::is_same_v<T, Database>) {
        return _database;
    } else if constexpr (std::is_same_v<T, Server>) {
        return _server;
    } else if constexpr (std::is_same_v<T, Waves>) {
        return _waves;
    } else {
        throw Exception::GenericError(std::format("Unsupported configuration type requested: {}", typeid(T).name()));
    }
}

void Misc::Env::LoadDatabaseFromFile(const libconfig::Setting& setting)
{
    if (!setting.isGroup()) {
        throw Exception::GenericError("Database configuration is not a group");
    }

    try {
        std::int32_t port = setting.lookup("port");

        _database.password = std::string(setting.lookup("password").c_str());
        _database.dbname = std::string(setting.lookup("dbname").c_str());
        _database.host = std::string(setting.lookup("host").c_str());
        _database.user = std::string(setting.lookup("user").c_str());
        _database.port = static_cast<std::uint16_t>(port);
    } catch (const libconfig::SettingNotFoundException& ex) {
        throw Exception::GenericError(std::format("Missing configuration parameter: {}", ex.getPath()));
    } catch (const libconfig::SettingTypeException& ex) {
        throw Exception::GenericError(std::format("Invalid type for configuration parameter: {}", ex.getPath()));
    }
}

void Misc::Env::LoadServerFromFile(const libconfig::Setting& setting)
{
    if (!setting.isGroup()) {
        throw Exception::GenericError("Server configuration is not a group");
    }

    try {
        std::int32_t port = setting.lookup("port");

        _server.port = static_cast<std::uint16_t>(port);
    } catch (const libconfig::SettingNotFoundException& ex) {
        throw Exception::GenericError(std::format("Missing configuration parameter: {}", ex.getPath()));
    } catch (const libconfig::SettingTypeException& ex) {
        throw Exception::GenericError(std::format("Invalid type for configuration parameter: {}", ex.getPath()));
    }
}

void Misc::Env::LoadWavesFromFile(const libconfig::Setting& setting)
{
    if (!setting.isArray()) {
        throw Exception::GenericError("Waves configuration is not an array");
    }

    try {
        for (std::int32_t i = 0; i < setting.getLength(); ++i) {
            std::string wavePath = setting[i].c_str();

            if (!std::filesystem::exists(wavePath)) {
                throw Exception::GenericError(std::format("Wave file does not exist: {}", wavePath));
            }

            _waves.files.push_back(wavePath);
        }
    } catch (const libconfig::SettingTypeException& ex) {
        throw Exception::GenericError(std::format("Invalid type for waves configuration: {}", ex.getPath()));
    }
}

template const Misc::Env::Database& Misc::Env::GetConfiguration<Misc::Env::Database>() const;

template const Misc::Env::Server& Misc::Env::GetConfiguration<Misc::Env::Server>() const;

template const Misc::Env::Waves& Misc::Env::GetConfiguration<Misc::Env::Waves>() const;
