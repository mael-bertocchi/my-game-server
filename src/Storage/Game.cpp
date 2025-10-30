/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Game.cpp
*/

#include "Exception/Game/AlreadyExists.hpp"
#include "Exception/Game/NotExists.hpp"
#include "Storage/Game.hpp"

const Storage::Cache::Game::IdToGameMap& Storage::Cache::Game::GetIdToGameMap() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    return _idToGame;
}

std::vector<std::uint32_t> Storage::Cache::Game::GetGameIds() const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    std::vector<std::uint32_t> ids = {};
    ids.reserve(_idToGame.size());
    for (const auto& pair : _idToGame) {
        ids.push_back(pair.first);
    }
    return ids;
}

void Storage::Cache::Game::AddGame(std::shared_ptr<Engine::Game> game)
{
    if (game) {
        const std::uint32_t id = game->GetId();

        std::unique_lock<std::shared_mutex> lock(_mutex);
        if (_idToGame.find(id) != _idToGame.end()) {
            throw Exception::Game::AlreadyExistsError(id);
        }
        _idToGame.emplace(id, game);
    }
}

std::shared_ptr<Engine::Game> Storage::Cache::Game::GetGameById(const std::uint32_t id) const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    auto it = _idToGame.find(id);
    if (it != _idToGame.end()) {
        return it->second;
    }
    throw Exception::Game::NotExistsError(id);
}

std::shared_ptr<Engine::Game> Storage::Cache::Game::GetGameByPlayerId(const std::uint32_t id) const
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    auto playerToGameIt = _playerToGame.find(id);
    if (playerToGameIt != _playerToGame.end()) {
        auto gameIt = _idToGame.find(playerToGameIt->second);
        if (gameIt != _idToGame.end()) {
            return gameIt->second;
        }
    }
    return nullptr;
}

void Storage::Cache::Game::AddPlayerToGame(const std::uint32_t playerId, const std::uint32_t gameId)
{
    std::unique_lock<std::shared_mutex> lock(_mutex);
    _playerToGame[playerId] = gameId;
}

void Storage::Cache::Game::RemovePlayerFromGame(const std::uint32_t playerId)
{
    std::unique_lock<std::shared_mutex> lock(_mutex);
    _playerToGame.erase(playerId);
}

void Storage::Cache::Game::RemoveGame(const std::uint32_t id)
{
    std::unique_lock<std::shared_mutex> lock(_mutex);
    auto it = _idToGame.find(id);
    if (it != _idToGame.end()) {
        for (auto playerIt = _playerToGame.begin(); playerIt != _playerToGame.end(); ) {
            if (playerIt->second != id) {
                ++playerIt;
            } else {
                playerIt = _playerToGame.erase(playerIt);
            }
        }
        _idToGame.erase(it);
    } else {
        throw Exception::Game::NotExistsError(id);
    }
}
