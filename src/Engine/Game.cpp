/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Game.cpp
*/

#include "Miscellaneous/Logger.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Exception/Generic.hpp"
#include "Action/Dispatcher.hpp"
#include "Engine/Collision.hpp"
#include "Action/List/STP.hpp"
#include "Storage/Player.hpp"
#include "Network/Player.hpp"
#include "Engine/Game.hpp"
#include "Variables.hpp"
#include "Types.hpp"

#include <tuple>
#include <unordered_map>
#include <functional>
#include <optional>
#include <cstdint>
#include <memory>

Engine::Game::Game() : _ids({0}), _wave(nullptr), _id(Misc::Utils::GetNextId("game")), _inactive(false), _started(false)
{
    _clocks = {
        { TimedEvent::Inactivity, Misc::Clock() },
        { TimedEvent::Wave, Misc::Clock() },
        { TimedEvent::Move, Misc::Clock() }
    };
    Misc::Logger::Log(std::format("Game {} created", _id));
}

Engine::Game::~Game()
{
    Misc::Logger::Log(std::format("Game {} destroyed", _id));
}

std::uint32_t Engine::Game::GetId() const
{
    return _id;
}

bool Engine::Game::AddPlayerId(const std::uint32_t id)
{
    std::int8_t slot = GetNextAvailablePlayerIdSlot(id);

    if (slot != -1) {
        std::shared_ptr<Network::Player> player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        if (player) {
            const std::uint16_t y = (WINDOW_HEIGHT / MAX_PLAYER_PER_GAMES) * slot + (WINDOW_HEIGHT / MAX_PLAYER_PER_GAMES) / 2;

            _ids[slot] = id;

            player->SetPosition({0, y});
            player->SetPlaying(true);
            player->SetAlive(true);

            Misc::Logger::Log(std::format("Player {} added to game {} at slot {}", id, _id, slot));

            for (const std::uint32_t& current : _ids) {
                if (current != 0) {
                    Action::Dispatcher::SendMessage(ActionType::JON, current, id);
                    if (current != id) {
                        Action::Dispatcher::SendMessage(ActionType::JON, id, current);
                    }
                }
            }
            return true;
        }
    }
    return false;
}

bool Engine::Game::RemovePlayerId(const std::uint32_t id)
{
    std::int8_t slot = GetPlayerIdSlot(id);

    if (slot != -1) {
        std::shared_ptr<Network::Player> player = Storage::Cache::Player::GetInstance().GetPlayerById(id);

        if (player) {
            player->SetPlaying(false);
            player->SetAlive(false);
        }

        for (const std::uint32_t& current : _ids) {
            if (current != 0 && current != id) {
                Action::Dispatcher::SendMessage(ActionType::LVE, current, id);
            }
        }
        _ids[slot] = 0;

        Misc::Logger::Log(std::format("Player {} removed from game {}", id, _id));
        return true;
    } else {
        return false;
    }
}

bool Engine::Game::KillPlayerId(const std::uint32_t id)
{
    if (GetPlayerIdSlot(id) != -1) {
        std::shared_ptr<Network::Player> player = Storage::Cache::Player::GetInstance().GetPlayerById(id);

        if (player) {
            player->SetAlive(false);
        }

        for (const std::uint32_t& current : _ids) {
            if (current != 0) {
                Action::Dispatcher::SendMessage(ActionType::DIE, current, std::make_pair(id, Misc::Utils::GetEnumIndex(CharacterType::Player)));
            }
        }
        return true;
    }
    return false;
}

std::array<std::uint32_t, MAX_PLAYER_PER_GAMES> Engine::Game::GetPlayerIds() const
{
    return _ids;
}

std::unordered_map<std::uint32_t, Entity>& Engine::Game::GetMissiles(const MissileType type)
{
    switch (type) {
        case MissileType::Player:
            return _missiles.player;
        case MissileType::Enemy:
            return _missiles.enemy;
        case MissileType::Force:
            return _missiles.force;
        default:
            throw Exception::GenericError(std::format("Invalid missile type, got {}", Misc::Utils::GetEnumIndex(type)));
    }
}

std::optional<std::reference_wrapper<Entity>> Engine::Game::GetMissile(const std::uint32_t id, const MissileType type)
{
    std::unordered_map<std::uint32_t, Entity>& missiles = GetMissiles(type);
    auto it = missiles.find(id);

    if (it != missiles.end()) {
        return std::ref(it->second);
    }
    return std::nullopt;
}

std::uint32_t Engine::Game::CreateMissile(Position position, const MissileType type)
{
    Entity missile = { .position = position, .id = 0, .health = 0 };

    switch (type) {
        case MissileType::Player:
            missile = { position, Misc::Utils::GetNextId(std::format("game-{}-player-missile", _id)), 0 };
            _missiles.player[missile.id] = missile;
            break;
        case MissileType::Enemy:
            missile = { position, Misc::Utils::GetNextId(std::format("game-{}-enemy-missile", _id)), 0 };
            _missiles.enemy[missile.id] = missile;
            break;
        case MissileType::Force:
            missile = { position, Misc::Utils::GetNextId(std::format("game-{}-player-force-missile", _id)), 0 };
            _missiles.force[missile.id] = missile;
            break;
        default:
            throw Exception::GenericError(std::format("Invalid missile type, got {}", Misc::Utils::GetEnumIndex(type)));
    }
    for (const std::uint32_t& current : _ids) {
        if (current != 0) {
            Action::Dispatcher::SendMessage(ActionType::SPW, current, std::make_tuple(missile.id, Misc::Utils::GetEnumIndex(type), missile.position));
        }
    }
    return missile.id;
}

void Engine::Game::MoveMissile(std::uint32_t id, const std::int16_t dx, const std::int16_t dy)
{
    std::optional<std::reference_wrapper<Entity>> opt = GetMissile(id, MissileType::Enemy);

    if (opt.has_value()) {
        Entity& missile = opt->get();

        std::int32_t newX = static_cast<std::int32_t>(missile.position.x) + dx;
        std::int32_t newY = static_cast<std::int32_t>(missile.position.y) + dy;

        if (newX < 0) {
            RemoveMissile(id, MissileType::Enemy);
        } else {
            if (newX > WINDOW_WIDTH) {
                newX = WINDOW_WIDTH;
            } else if (newY < 0) {
                newY = 0;
            } else if (newY > WINDOW_HEIGHT) {
                newY = WINDOW_HEIGHT;
            }

            missile.position.x = static_cast<std::uint16_t>(newX);
            missile.position.y = static_cast<std::uint16_t>(newY);

            QueuePosition(missile.id, Misc::Utils::GetEnumIndex(MissileType::Enemy), missile.position);
        }
    }
}

bool Engine::Game::MoveMissile(Entity& missile, const MissileType type)
{
    if (missile.position.x > WINDOW_WIDTH) {
        return true;
    }
    missile.position.x += MISSILE_MOVE_SPEED;
    QueuePosition(missile.id, Misc::Utils::GetEnumIndex(type), missile.position);
    return false;
}

void Engine::Game::RemoveMissile(const std::uint32_t id, const MissileType type)
{
    std::unordered_map<std::uint32_t, Entity>& missiles = GetMissiles(type);
    auto it = missiles.find(id);

    if (it != missiles.end()) {
        missiles.erase(it);

        for (const std::uint32_t& current : _ids) {
            if (current != 0) {
                Action::Dispatcher::SendMessage(ActionType::DIE, current, std::make_pair(id, Misc::Utils::GetEnumIndex(type)));
            }
        }
    }
}

bool Engine::Game::IsStarted() const
{
    return _started;
}

bool Engine::Game::IsInactive()
{
    if (GetPlayerCount() > 0) {
        _inactive = false;
        return false;
    }
    if (!_inactive) {
        _clocks.at(TimedEvent::Inactivity).Reset();
        _inactive = true;
        return false;
    }
    if (_clocks.at(TimedEvent::Inactivity).HasElapsed(GAME_INACTIVITY_TIMEOUT_MS)) {
        Misc::Logger::Log(std::format("Game {} marked as inactive", _id));
        return true;
    }
    return false;
}

void Engine::Game::Process()
{
    if (_started) {
        Wave::Result result = Wave::Result::Continue;

        if (_wave) {
            const float dt = _clocks.at(TimedEvent::Wave).GetElapsedTimeInSeconds();
            result = _wave->Process(dt);
        }
        switch (result) {
            case Wave::Result::Continue:
                MoveEntities();
                SendPosition();
                break;
            case Wave::Result::Next:
                Next();
                break;
            case Wave::Result::Stop:
                Stop();
                break;
        }
        _clocks.at(TimedEvent::Wave).Reset();
    }
}

void Engine::Game::MoveEntities()
{
    if (_clocks.at(TimedEvent::Move).HasElapsed(ENTITY_MOVE_INTERVAL_MS)) {
        std::vector<std::uint32_t> missiles = {};

        for (auto& [id, missile] : _missiles.player) {
            if (MoveMissile(missile, MissileType::Player)) {
                missiles.push_back(id);
            }
        }

        for (const auto& id : missiles) {
            RemoveMissile(id, MissileType::Player);
        }

        missiles.clear();

        for (auto& [id, missile] : _missiles.force) {
            if (MoveMissile(missile, MissileType::Force)) {
                missiles.push_back(id);
            }
        }

        for (const auto& id : missiles) {
            RemoveMissile(id, MissileType::Force);
        }

        for (const std::uint32_t& current : _ids) {
            if (current != 0) {
                const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(current);
                if (player) {
                    if (player->IsStatisticGone(Network::Player::Statistic::Shield)) {
                        SetPlayerIdStatistic(player, Network::Player::Statistic::Shield, false);
                    }
                    if (player->IsStatisticGone(Network::Player::Statistic::Force)) {
                        SetPlayerIdStatistic(player, Network::Player::Statistic::Force, false);
                    }
                }
            }
        }

        Collision::Result result = Collision::Check(_ids, _enemies, _missiles, _items);
        ApplyCollisions(result);

        _clocks.at(TimedEvent::Move).Reset();
    }
}

void Engine::Game::Start()
{
    if (!_started) {
        std::vector<std::pair<std::uint32_t, Position>> positions = {};

        _wave = std::make_unique<Wave>(_id);
        for (const std::uint32_t& current : _ids) {
            if (current != 0) {
                const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(current);
                if (player) {
                    positions.push_back({ current, player->GetPosition() });
                }
            }
        }
        for (const auto& position : positions) {
            Action::Dispatcher::SendMessage(ActionType::STR, position.first, positions);
        }
        _started = true;
    }
}

void Engine::Game::Stop()
{
    if (_started) {
        Action::List::STP::Result result = Action::List::STP::Result::Lose;

        for (const std::uint32_t& current : _ids) {
            if (current != 0) {
                const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(current);
                if (player && player->IsAlive()) {
                    result = Action::List::STP::Result::Win;
                    break;
                }
            }
        }
        for (const std::uint32_t& current : _ids) {
            if (current != 0) {
                Action::Dispatcher::SendMessage(ActionType::STP, current, result);
                RemovePlayerId(current);
            }
        }
        _started = false;
    }
}

void Engine::Game::Next()
{
    for (const std::uint32_t& current : _ids) {
        if (current != 0) {
            Action::Dispatcher::SendMessage(ActionType::NXT, current);
        }
    }
    _positions.clear();
}

void Engine::Game::QueuePosition(const std::uint32_t id, std::uint8_t type, const Position position)
{
    if (_positions.size() < std::numeric_limits<std::uint16_t>::max()) {
        _positions.push_back(std::make_tuple(id, type, position));
    }
}

void Engine::Game::SendPosition()
{
    if (!_positions.empty()) {
        for (const std::uint32_t& current : _ids) {
            if (current != 0) {
                Action::Dispatcher::SendMessage(ActionType::POS, current, _positions);
            }
        }
        _positions.clear();
    }
}

std::int8_t Engine::Game::GetPlayerIdSlot(const std::uint32_t id) const
{
    for (std::int8_t i = 0; i < MAX_PLAYER_PER_GAMES; i++) {
        if (_ids[i] != id) {
            continue;
        }
        return i;
    }
    return -1;
}

std::int8_t Engine::Game::GetNextAvailablePlayerIdSlot(const std::uint32_t id) const
{
    if (GetPlayerIdSlot(id) != -1) {
        return -1;
    }
    for (std::int8_t i = 0; i < MAX_PLAYER_PER_GAMES; i++) {
        if (_ids[i] != 0) {
            continue;
        }
        return i;
    }
    return -1;
}

std::uint8_t Engine::Game::GetPlayerCount() const
{
    std::uint8_t count = 0;

    for (const auto& id : _ids) {
        if (id != 0) {
            count++;
        }
    }
    return count;
}

std::unordered_map<std::uint32_t, Entity>& Engine::Game::GetEnemies(const EnemyType type)
{
    switch (type) {
        case EnemyType::Generic:
            return _enemies.generic;
        case EnemyType::Walking:
            return _enemies.walking;
        case EnemyType::Flying:
            return _enemies.flying;
        default:
            throw Exception::GenericError(std::format("Invalid enemy type, got {}", Misc::Utils::GetEnumIndex(type)));
    }
}

std::optional<std::reference_wrapper<Entity>> Engine::Game::GetEnemy(const std::uint32_t id, const EnemyType type)
{
    std::unordered_map<std::uint32_t, Entity>& enemies = GetEnemies(type);
    auto it = enemies.find(id);

    if (it != enemies.end()) {
        return std::ref(it->second);
    }
    return std::nullopt;
}

std::uint32_t Engine::Game::CreateEnemy(const Position position, const EnemyType type)
{
    std::unordered_map<std::uint32_t, Entity>& enemies = GetEnemies(type);
    std::size_t size = _enemies.generic.size() + _enemies.walking.size() + _enemies.flying.size();

    if (size >= MAX_ENEMIES_PER_GAME) {
        throw Exception::GenericError(std::format("Maximum enemies reached in game {}", MAX_ENEMIES_PER_GAME, _id));
    }

    Entity enemy = { .position = position, .id = Misc::Utils::GetNextId(std::format("game-{}-enemy", _id)), .health = 100 };

    enemies[enemy.id] = enemy;

    for (const std::uint32_t& current : _ids) {
        if (current != 0) {
            Action::Dispatcher::SendMessage(ActionType::SPW, current, std::make_tuple(enemy.id, Misc::Utils::GetEnumIndex(type), enemy.position));
        }
    }

    return enemy.id;
}

void Engine::Game::MoveEnemy(const std::uint32_t id, const EnemyType type, const std::int16_t dx, const std::int16_t dy)
{
    std::optional<std::reference_wrapper<Entity>> opt = GetEnemy(id, type);

    if (opt.has_value()) {
        Entity& enemy = opt->get();

        std::int32_t newX = static_cast<std::int32_t>(enemy.position.x) + dx;
        std::int32_t newY = static_cast<std::int32_t>(enemy.position.y) + dy;

        if (newX < 0) {
            RemoveEnemy(id, type);
        } else {
            if (newX > WINDOW_WIDTH) {
                newX = WINDOW_WIDTH;
            } else if (newY < 0) {
                newY = 0;
            } else if (newY > WINDOW_HEIGHT) {
                newY = WINDOW_HEIGHT;
            }

            enemy.position.x = static_cast<std::uint16_t>(newX);
            enemy.position.y = static_cast<std::uint16_t>(newY);

            QueuePosition(enemy.id, Misc::Utils::GetEnumIndex(type), enemy.position);
        }
    }
}

void Engine::Game::RemoveEnemy(const std::uint32_t id, const EnemyType type)
{
    std::unordered_map<std::uint32_t, Entity>& enemies = GetEnemies(type);
    auto it = enemies.find(id);

    if (it != enemies.end()) {
        enemies.erase(it);

        for (const std::uint32_t& current : _ids) {
            if (current != 0) {
                Action::Dispatcher::SendMessage(ActionType::DIE, current, std::make_pair(id, Misc::Utils::GetEnumIndex(type)));
            }
        }
    }
}

bool Engine::Game::DamageEnemy(const std::uint32_t id, const EnemyType type, const std::int32_t damage)
{
    std::unordered_map<std::uint32_t, Entity>& enemies = GetEnemies(type);
    auto it = enemies.find(id);

    if (it != enemies.end()) {
        it->second.health -= damage;

        if (it->second.health <= 0) {
            RemoveEnemy(id, type);
            return true;
        }
    }
    return false;
}

void Engine::Game::ApplyCollisions(const Collision::Result& result)
{
    for (const auto& [enemyId, damage] : result.damaged.generic) {
        DamageEnemy(enemyId, EnemyType::Generic, damage);
    }
    for (const auto& [enemyId, damage] : result.damaged.walking) {
        DamageEnemy(enemyId, EnemyType::Walking, damage);
    }
    for (const auto& [enemyId, damage] : result.damaged.flying) {
        DamageEnemy(enemyId, EnemyType::Flying, damage);
    }
    for (const std::uint32_t id : result.enemies.generic) {
        RemoveEnemy(id, EnemyType::Generic);
    }
    for (const std::uint32_t id : result.enemies.walking) {
        RemoveEnemy(id, EnemyType::Walking);
    }
    for (const std::uint32_t id : result.enemies.flying) {
        RemoveEnemy(id, EnemyType::Flying);
    }
    for (const std::uint32_t id : result.missiles.player) {
        RemoveMissile(id, MissileType::Player);
    }
    for (const std::uint32_t id : result.missiles.force) {
        RemoveMissile(id, MissileType::Force);
    }
    for (const std::uint32_t id : result.missiles.enemy) {
        RemoveMissile(id, MissileType::Enemy);
    }
    for (const auto& [shieldId, playerId] : result.shields) {
        RemoveItem(shieldId, ItemType::Shield);
        SetPlayerIdStatistic(playerId, Network::Player::Statistic::Shield, true);
    }
    for (const auto& [forceId, playerId] : result.forces) {
        RemoveItem(forceId, ItemType::Force);
        SetPlayerIdStatistic(playerId, Network::Player::Statistic::Force, true);
    }
    for (const std::uint32_t id : result.players) {
        KillPlayerId(id);
    }
}

std::unordered_map<std::uint32_t, Entity>& Engine::Game::GetItems(const ItemType type)
{
    switch (type) {
        case ItemType::Shield:
            return _items.shield;
        case ItemType::Force:
            return _items.force;
        default:
            throw Exception::GenericError(std::format("Invalid item type, got {}", Misc::Utils::GetEnumIndex(type)));
    }
}

std::optional<std::reference_wrapper<Entity>> Engine::Game::GetItem(const std::uint32_t id, const ItemType type)
{
    std::unordered_map<std::uint32_t, Entity>& items = GetItems(type);
    auto it = items.find(id);

    if (it != items.end()) {
        return std::ref(it->second);
    }
    return std::nullopt;
}

std::uint32_t Engine::Game::CreateItem(const Position position, const ItemType type)
{
    Entity item = { .position = position, .id = 0, .health = 0 };

    switch (type) {
        case ItemType::Shield:
            item = { position, Misc::Utils::GetNextId(std::format("game-{}-shield-item", _id)), 0 };
            _items.shield[item.id] = item;
            break;
        case ItemType::Force:
            item = { position, Misc::Utils::GetNextId(std::format("game-{}-force-item", _id)), 0 };
            _items.force[item.id] = item;
            break;
        default:
            throw Exception::GenericError(std::format("Invalid item type, got {}", Misc::Utils::GetEnumIndex(type)));
    }
    for (const std::uint32_t& current : _ids) {
        if (current != 0) {
            Action::Dispatcher::SendMessage(ActionType::SPW, current, std::make_tuple(item.id, Misc::Utils::GetEnumIndex(type), item.position));
        }
    }
    return item.id;
}

void Engine::Game::RemoveItem(const std::uint32_t id, const ItemType type)
{
    std::unordered_map<std::uint32_t, Entity>& items = GetItems(type);
    auto it = items.find(id);

    if (it != items.end()) {
        items.erase(it);

        for (const std::uint32_t& current : _ids) {
            if (current != 0) {
                Action::Dispatcher::SendMessage(ActionType::DIE, current, std::make_pair(id, Misc::Utils::GetEnumIndex(type)));
            }
        }
    }
}

void Engine::Game::SetPlayerIdStatistic(const std::uint32_t id, const Network::Player::Statistic& statistic, const bool status)
{
    std::shared_ptr<Network::Player> player = Storage::Cache::Player::GetInstance().GetPlayerById(id);

    if (player) {
        player->SetStatistic(statistic, status);
        for (const std::uint32_t& current : _ids) {
            if (current != 0) {
                Action::Dispatcher::SendMessage(ActionType::STS, current, std::make_tuple(id, statistic, status));
            }
        }
    }
}

void Engine::Game::SetPlayerIdStatistic(const std::shared_ptr<Network::Player>& player, const Network::Player::Statistic& statistic, const bool status)
{
    if (player) {
        player->SetStatistic(statistic, status);
        for (const std::uint32_t& current : _ids) {
            if (current != 0) {
                Action::Dispatcher::SendMessage(ActionType::STS, current, std::make_tuple(player->GetId(), statistic, status));
            }
        }
    }
}
