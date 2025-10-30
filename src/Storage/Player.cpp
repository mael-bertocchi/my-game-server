/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Player.cpp
*/

#include "Exception/Socket/AlreadyRegistered.hpp"
#include "Exception/Socket/NotRegistered.hpp"
#include "Storage/Player.hpp"

#include <shared_mutex>
#include <format>

const Storage::Cache::Player::SocketToPlayerMap& Storage::Cache::Player::GetSocketToPlayerMap() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    return _socketToPlayer;
}

const Storage::Cache::Player::IdToPlayerMap& Storage::Cache::Player::GetIdToPlayerMap() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    return _idToPlayer;
}

const std::shared_ptr<Network::Player> Storage::Cache::Player::GetPlayerBySocket(const Wrapper::Socket::SocketType socket) const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    auto it = _socketToPlayer.find(socket);
    if (it != _socketToPlayer.end()) {
        return it->second;
    }
    return nullptr;
}

const std::shared_ptr<Network::Player> Storage::Cache::Player::GetPlayerById(const std::uint32_t id) const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    auto it = _idToPlayer.find(id);
    if (it != _idToPlayer.end()) {
        return it->second;
    }
    return nullptr;
}

const std::shared_ptr<Network::Player> Storage::Cache::Player::GetPlayerByAddress(const std::string& address, const std::uint16_t port) const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    auto it = _addressToPlayer.find(std::format("{}:{}", address, port));
    if (it != _addressToPlayer.end()) {
        return it->second;
    }
    return nullptr;
}

std::optional<Wrapper::Socket::SocketType> Storage::Cache::Player::GetSocketByPlayerId(const std::uint32_t id) const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    auto it = _idToSocket.find(id);
    if (it != _idToSocket.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<std::uint32_t> Storage::Cache::Player::GetPlayerIdBySocket(const Wrapper::Socket::SocketType socket) const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    auto it = _socketToPlayer.find(socket);
    if (it != _socketToPlayer.end()) {
        return it->second->GetId();
    }
    return std::nullopt;
}

void Storage::Cache::Player::AddPlayer(const Wrapper::Socket::SocketType socket, std::shared_ptr<Network::Player> player)
{
    std::unique_lock<std::shared_mutex> lock(_mutex);
    if (_socketToPlayer.find(socket) != _socketToPlayer.end()) {
        throw Exception::Socket::AlreadyRegisteredError(socket);
    }
    const std::string address = std::format("{}:{}", player->GetAddress(), player->GetPort());
    const std::uint32_t id = player->GetId();

    _addressToPlayer.emplace(address, player);
    _socketToPlayer.emplace(socket, player);
    _idToPlayer.emplace(id, player);
    _idToSocket.emplace(id, socket);
}

std::uint32_t Storage::Cache::Player::RemovePlayer(const Wrapper::Socket::SocketType socket)
{
    std::unique_lock<std::shared_mutex> lock(_mutex);
    auto it = _socketToPlayer.find(socket);
    if (it != _socketToPlayer.end()) {
        const std::string address = std::format("{}:{}", it->second->GetAddress(), it->second->GetPort());
        const std::uint32_t id = it->second->GetId();

        _addressToPlayer.erase(address);
        _socketToPlayer.erase(socket);
        _idToPlayer.erase(id);
        _idToSocket.erase(id);

        return id;
    }
    throw Exception::Socket::NotRegisteredError(socket);
}
