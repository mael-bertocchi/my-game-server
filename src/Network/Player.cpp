/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Player.cpp
*/

#include "Exception/EmptyMessageQueue.hpp"
#include "Miscellaneous/Password.hpp"
#include "Miscellaneous/Logger.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Storage/Database.hpp"
#include "Network/Player.hpp"
#include "Variables.hpp"
#include "Types.hpp"

#include <cctype>
#include <format>

Network::Player::Player(const std::string& address, const std::uint16_t port) : _address(address), _port(port), _id(Misc::Utils::GetNextId("player")), _position({0, 0}), _role(Role::Player), _playing(false), _alive(true), _god(false)
{
    _statistics = {
        { Statistic::Shield, { Misc::Clock(), false } },
        { Statistic::Force, { Misc::Clock(), false } }
    };
    Misc::Logger::Log(std::format("Player {} connected from {}:{}", _id, address, port));
}

Network::Player::~Player()
{
    Misc::Logger::Log(std::format("Player {} disconnected", _id));
}

void Network::Player::PushMessage(const Wrapper::Socket::Protocol& protocol, const Message& message)
{
    if (protocol != Wrapper::Socket::Protocol::TCP) {
        std::lock_guard<std::mutex> lock(_udpMutex);
        _udp.push(message);
    } else {
        std::lock_guard<std::mutex> lock(_tcpMutex);
        _tcp.push(message);
    }
}

const Network::Player::Message Network::Player::PopMessage(const Wrapper::Socket::Protocol& protocol)
{
    if (protocol != Wrapper::Socket::Protocol::TCP) {
        std::lock_guard<std::mutex> lock(_udpMutex);
        if (_udp.empty()) {
            throw Exception::EmptyMessageQueueError();
        }
        Message message = _udp.front();
        _udp.pop();
        return message;
    } else {
        std::lock_guard<std::mutex> lock(_tcpMutex);
        if (_tcp.empty()) {
            throw Exception::EmptyMessageQueueError();
        }
        Message message = _tcp.front();
        _tcp.pop();
        return message;
    }
}

bool Network::Player::HasMessage(const Wrapper::Socket::Protocol& protocol) const
{
    if (protocol != Wrapper::Socket::Protocol::TCP) {
        std::lock_guard<std::mutex> lock(_udpMutex);
        return !_udp.empty();
    } else {
        std::lock_guard<std::mutex> lock(_tcpMutex);
        return !_tcp.empty();
    }
}

bool Network::Player::DoesExist(const std::string& username) const
{
    try {
        auto transaction = Storage::Database::GetInstance().CreateTransaction();
        const pqxx::result result = transaction->exec("SELECT 1 FROM players WHERE username = $1 LIMIT 1", pqxx::params{username});

        return !result.empty();
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to check existence of username '{}' for player {}: {}", username, _id, ex.what()), Misc::Logger::LogLevel::Critical);
        return false;
    } catch (...) {
        Misc::Logger::Log(std::format("Unknown error while checking existence of username '{}' for player {}", username, _id), Misc::Logger::LogLevel::Critical);
        return false;
    }
}

bool Network::Player::Connect(const std::string& username, const std::string& password) {
    try {
        auto transaction = Storage::Database::GetInstance().CreateTransaction();
        const pqxx::result result = transaction->exec("SELECT password_hash, role FROM players WHERE username = $1", pqxx::params{username});

        if (result.empty()) {
            Misc::Logger::Log(std::format("Username '{}' not found for player {}", username, _id), Misc::Logger::LogLevel::Caution);
            return false;
        }

        const std::string stored_hash = result[0]["password_hash"].as<std::string>();
        const std::string role = result[0]["role"].as<std::string>();

        if (Misc::Password::VerifyPassword(password, stored_hash)) {
            Misc::Logger::Log(std::format("Player {} successfully connected as '{}'", _id, username));
            if (role != "player") {
                _role = Role::Administrator;
            }
            _username = username;
            return true;
        }

        Misc::Logger::Log(std::format("Invalid password attempt for username '{}' from player {}", username, _id), Misc::Logger::LogLevel::Caution);
        return false;
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to connect player {} as '{}': {}", _id, username, ex.what()), Misc::Logger::LogLevel::Critical);
        return false;
    } catch (...) {
        Misc::Logger::Log(std::format("Unknown error while connecting player {} as '{}'", _id, username), Misc::Logger::LogLevel::Critical);
        return false;
    }
}

bool Network::Player::Register(const std::string& username, const std::string& password) {
    try {
        if (DoesExist(username)) {
            Misc::Logger::Log(std::format("Registration failed: username '{}' already exists for player {}", username, _id), Misc::Logger::LogLevel::Caution);
            return false;
        }

        auto transaction = Storage::Database::GetInstance().CreateTransaction();

        const std::string password_hash = Misc::Password::HashPassword(password);
        const pqxx::result result = transaction->exec("INSERT INTO players (username, password_hash) VALUES ($1, $2) RETURNING id", pqxx::params{username, password_hash});

        if (result.empty()) {
            Misc::Logger::Log(std::format("Failed to insert new player '{}' (player {})", username, _id), Misc::Logger::LogLevel::Critical);
            return false;
        }

        transaction->commit();
        _username = username;

        Misc::Logger::Log(std::format("Registered new player {} with username '{}'", _id, username));

        return true;
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Exception during registration of '{}' for player {}: {}", username, _id, ex.what()), Misc::Logger::LogLevel::Critical);
        return false;
    } catch (...) {
        Misc::Logger::Log(std::format("Unknown error while registering '{}' for player {}", username, _id), Misc::Logger::LogLevel::Critical);
        return false;
    }
}

const Position& Network::Player::Move(const Direction& direction)
{
    Misc::Maths::Vector2<std::int8_t> offset = {0, 0};

    switch (direction) {
        case Direction::Up:
            offset.y = -PLAYER_MOVE_SPEED;
            break;
        case Direction::Down:
            offset.y = PLAYER_MOVE_SPEED;
            break;
        case Direction::Left:
            offset.x = -PLAYER_MOVE_SPEED;
            break;
        case Direction::Right:
            offset.x = PLAYER_MOVE_SPEED;
            break;
        case Direction::UpRight:
            offset.x = PLAYER_MOVE_SPEED;
            offset.y = -PLAYER_MOVE_SPEED;
            break;
        case Direction::UpLeft:
            offset.x = -PLAYER_MOVE_SPEED;
            offset.y = -PLAYER_MOVE_SPEED;
            break;
        case Direction::DownRight:
            offset.x = PLAYER_MOVE_SPEED;
            offset.y = PLAYER_MOVE_SPEED;
            break;
        case Direction::DownLeft:
            offset.x = -PLAYER_MOVE_SPEED;
            offset.y = PLAYER_MOVE_SPEED;
            break;
        default:
            break;
    }
    if (_position.x + offset.x >= 0 && _position.x + offset.x <= WINDOW_WIDTH) {
        _position.x += offset.x;
    }
    if (_position.y + offset.y >= 0 && _position.y + offset.y <= WINDOW_HEIGHT) {
        _position.y += offset.y;
    }
    return _position;
}

bool Network::Player::IsConnected() const
{
    return !_username.empty();
}

const std::string& Network::Player::GetAddress() const
{
    return _address;
}

std::uint16_t Network::Player::GetPort() const
{
    return _port;
}

std::uint32_t Network::Player::GetId() const
{
    return _id;
}

void Network::Player::SetPlaying(bool status)
{
    _playing = status;
}

bool Network::Player::IsPlaying() const
{
    return _playing;
}

void Network::Player::SetAlive(bool status)
{
    _alive = status;
}

bool Network::Player::IsAlive() const
{
    return _alive;
}

void Network::Player::SetPosition(const Position& position)
{
    _position = position;
}

const Position& Network::Player::GetPosition() const
{
    return _position;
}

const Role& Network::Player::GetRole() const
{
    return _role;
}

void Network::Player::SetStatistic(const Statistic& statistic, const bool status, const bool god)
{
    const std::string str = StatisticToString(statistic);

    if (!god && _god) {
        Misc::Logger::Log(std::format("When god mode is active, you must call this function with god paramaeter set to modify {} statistic for player {}", str, _id));
    } else {
        Misc::Logger::Log(std::format("{} statistic {} for player {}", status ? "Activate" : "Deactivate", str, _id));
        if (status) {
            _statistics[statistic].first.Reset();
            _god = god;
        } else {
            _god = false;
        }
        _statistics[statistic].second = status;
    }
}

bool Network::Player::IsStatisticActive(const Statistic& statistic) const
{
    return _statistics.at(statistic).second;
}

bool Network::Player::IsStatisticGone(const Statistic& statistic) const
{
    if (_god) {
        return false;
    }
    const bool elapsed = _statistics.at(statistic).first.HasElapsed(PLAYER_STATISTIC_DURATION_MS);

    if (elapsed) {
        Misc::Logger::Log(std::format("{} statistic expired for player {}", StatisticToString(statistic), _id));
    }
    return elapsed;
}

const std::string Network::Player::StatisticToString(const Statistic& statistic) const
{
    switch (statistic) {
        case Statistic::Shield:
            return "shield";
        case Statistic::Force:
            return "force";
        default:
            return "unknown";
    }
}

const std::string Network::Player::RoleToString(const Role& role) const
{
    switch (role) {
        case Role::Administrator:
            return "administrator";
        case Role::Player:
            return "player";
        default:
            return "unknown";
    }
}
