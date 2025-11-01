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

#include <unordered_map>
#include <functional>
#include <optional>
#include <cstdint>
#include <memory>
#include <tuple>

/**
 * Prevent Windows from defining min and max macros that interfere with min and max
 */
#ifndef NOMINMAX
    #define NOMINMAX
#endif

/**
 * Prevent Windows from defining max macro that interferes with max functionality
 */
#ifdef max
    #undef max
#endif

Engine::Game::Game() : _ids({0}), _wave(nullptr), _id(Misc::Utils::GetNextId("game")), _inactive(false), _started(false)
{
    _clocks = {
        { TimedEvent::Inactivity, Misc::Clock() },
        { TimedEvent::Wave, Misc::Clock() },
        { TimedEvent::Move, Misc::Clock() }
    };
    Misc::Logger::Log(std::format("[Game — {}] Created", _id));
}

Engine::Game::~Game()
{
    Misc::Logger::Log(std::format("[Game — {}] Destroyed", _id));
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

            Misc::Logger::Log(std::format("[Game — {}] Player {} added at slot {}", _id, id, slot));

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

        Misc::Logger::Log(std::format("[Game — {}] Player {} removed", _id, id));
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

        Misc::Logger::Log(std::format("[Game — {}] Player {} killed", _id, id));
        for (const std::uint32_t& current : _ids) {
            if (current != 0) {
                Action::Dispatcher::SendMessage(ActionType::DIE, current, std::make_pair(id, Misc::Utils::GetEnumIndex(Character::Player)));
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

std::unordered_map<std::uint32_t, Entity>& Engine::Game::GetMissiles(const Missile type)
{
    switch (type) {
        case Missile::Player:
            return _missiles.player;
        case Missile::Enemy:
            return _missiles.enemy;
        case Missile::Force:
            return _missiles.force;
        case Missile::Boss:
            return _missiles.boss;
        default:
            throw Exception::GenericError(std::format("Invalid missile type, got {}", Misc::Utils::GetEnumIndex(type)));
    }
}

std::optional<std::reference_wrapper<Entity>> Engine::Game::GetMissile(const std::uint32_t id, const Missile type)
{
    std::unordered_map<std::uint32_t, Entity>& missiles = GetMissiles(type);
    auto it = missiles.find(id);

    if (it != missiles.end()) {
        return std::ref(it->second);
    }
    return std::nullopt;
}

std::uint32_t Engine::Game::CreateMissile(const Missile type, const Position position)
{
    Entity missile = { .position = position, .id = 0, .health = 0 };

    switch (type) {
        case Missile::Player:
            missile.id = Misc::Utils::GetNextId(std::format("game-{}-player-missile", _id));
            _missiles.player[missile.id] = missile;
            break;
        case Missile::Enemy:
            missile.id = Misc::Utils::GetNextId(std::format("game-{}-enemy-missile", _id));
            _missiles.enemy[missile.id] = missile;
            break;
        case Missile::Force:
            missile.id = Misc::Utils::GetNextId(std::format("game-{}-force-missile", _id));
            _missiles.force[missile.id] = missile;
            break;
        case Missile::Boss:
            missile.id = Misc::Utils::GetNextId(std::format("game-{}-boss-missile", _id));
            _missiles.boss[missile.id] = missile;
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

void Engine::Game::MoveMissile(const std::uint32_t id, const Missile type, const std::int16_t dx, const std::int16_t dy)
{
    std::optional<std::reference_wrapper<Entity>> opt = GetMissile(id, type);

    if (opt.has_value()) {
        Entity& missile = opt->get();

        std::int32_t newX = static_cast<std::int32_t>(missile.position.x) + dx;
        std::int32_t newY = static_cast<std::int32_t>(missile.position.y) + dy;

        if (newX < 0) {
            RemoveMissile(id, type);
        } else if (newY < 0) {
            RemoveMissile(id, type);
        } else if (newY > WINDOW_HEIGHT) {
            RemoveMissile(id, type);
        } else {
            if (newX > WINDOW_WIDTH) {
                newX = WINDOW_WIDTH;
            }

            missile.position.x = static_cast<std::uint16_t>(newX);
            missile.position.y = static_cast<std::uint16_t>(newY);

            QueuePosition(missile.id, Misc::Utils::GetEnumIndex(type), missile.position);
        }
    }
}

bool Engine::Game::MoveMissile(Entity& missile, const Missile type)
{
    if (missile.position.x > WINDOW_WIDTH) {
        return true;
    }
    missile.position.x += MISSILE_MOVE_SPEED;
    QueuePosition(missile.id, Misc::Utils::GetEnumIndex(type), missile.position);
    return false;
}

void Engine::Game::RemoveMissile(const std::uint32_t id, const Missile type)
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
        Misc::Logger::Log(std::format("[Game — {}] Inactivity timeout reached", _id));
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
            if (MoveMissile(missile, Missile::Player)) {
                missiles.push_back(id);
            }
        }

        for (const auto& id : missiles) {
            RemoveMissile(id, Missile::Player);
        }

        missiles.clear();

        for (auto& [id, missile] : _missiles.force) {
            if (MoveMissile(missile, Missile::Force)) {
                missiles.push_back(id);
            }
        }

        for (const auto& id : missiles) {
            RemoveMissile(id, Missile::Force);
        }

        for (const std::uint32_t& current : _ids) {
            if (current != 0) {
                const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(current);
                if (player) {
                    if (player->IsStatisticActive(Statistic::Shield) && player->IsStatisticGone(Statistic::Shield)) {
                        SetPlayerIdStatistic(player, Statistic::Shield, false);
                    }
                    if (player->IsStatisticActive(Statistic::Shield) && player->IsStatisticGone(Statistic::Force)) {
                        SetPlayerIdStatistic(player, Statistic::Force, false);
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
        Misc::Logger::Log(std::format("[Game — {}] Started with {} players", _id, positions.size()));
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
        Misc::Logger::Log(std::format("[Game — {}] Stoped, players {}", _id, Action::List::STP::ResultToString(result)));
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
    _missiles.player.clear();
    _missiles.enemy.clear();
    _missiles.force.clear();

    _enemies.generic.clear();
    _enemies.walking.clear();
    _enemies.flying.clear();

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

std::unordered_map<std::uint32_t, Entity>& Engine::Game::GetEnemies(const Enemy type)
{
    switch (type) {
        case Enemy::Generic:
            return _enemies.generic;
        case Enemy::Walking:
            return _enemies.walking;
        case Enemy::Flying:
            return _enemies.flying;
        case Enemy::Boss:
            return _enemies.boss;
        default:
            throw Exception::GenericError(std::format("Invalid enemy type, got {}", Misc::Utils::GetEnumIndex(type)));
    }
}

std::optional<std::reference_wrapper<Entity>> Engine::Game::GetEnemy(const std::uint32_t id, const Enemy type)
{
    std::unordered_map<std::uint32_t, Entity>& enemies = GetEnemies(type);
    auto it = enemies.find(id);

    if (it != enemies.end()) {
        return std::ref(it->second);
    }
    return std::nullopt;
}

std::uint32_t Engine::Game::CreateEnemy(const Enemy type, const Position position)
{
    Entity enemy = { .position = position, .id = 0, .health = 100 };

    switch (type) {
        case Enemy::Generic:
            enemy.id = Misc::Utils::GetNextId(std::format("game-{}-generic-enemy", _id));
            enemy.health = GENERIC_ENEMY_HEALTH;
            _enemies.generic[enemy.id] = enemy;
            break;
        case Enemy::Walking:
            enemy.id = Misc::Utils::GetNextId(std::format("game-{}-walking-enemy", _id));
            enemy.health = WALKING_ENEMY_HEALTH;
            _enemies.walking[enemy.id] = enemy;
            break;
        case Enemy::Flying:
            enemy.id = Misc::Utils::GetNextId(std::format("game-{}-flying-enemy", _id));
            enemy.health = FLYING_ENEMY_HEALTH;
            _enemies.flying[enemy.id] = enemy;
            break;
        case Enemy::Boss:
            enemy.id = Misc::Utils::GetNextId(std::format("game-{}-boss-enemy", _id));
            enemy.health = BOSS_ENEMY_HEALTH;
            _enemies.boss[enemy.id] = enemy;
            break;
        default:
            throw Exception::GenericError(std::format("Invalid enemy type, got {}", Misc::Utils::GetEnumIndex(type)));
    }
    for (const std::uint32_t& current : _ids) {
        if (current != 0) {
            Action::Dispatcher::SendMessage(ActionType::SPW, current, std::make_tuple(enemy.id, Misc::Utils::GetEnumIndex(type), enemy.position));
        }
    }
    return enemy.id;
}

void Engine::Game::MoveEnemy(const std::uint32_t id, const Enemy type, const std::int16_t dx, const std::int16_t dy)
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

void Engine::Game::RemoveEnemy(const std::uint32_t id, const Enemy type)
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

bool Engine::Game::DamageEnemy(const std::uint32_t id, const Enemy type, const std::int32_t damage)
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
        DamageEnemy(enemyId, Enemy::Generic, damage);
    }
    for (const auto& [enemyId, damage] : result.damaged.walking) {
        DamageEnemy(enemyId, Enemy::Walking, damage);
    }
    for (const auto& [enemyId, damage] : result.damaged.flying) {
        DamageEnemy(enemyId, Enemy::Flying, damage);
    }
    for (const auto& [enemyId, damage] : result.damaged.boss) {
        DamageEnemy(enemyId, Enemy::Boss, damage);
    }
    for (const std::uint32_t id : result.enemies.generic) {
        RemoveEnemy(id, Enemy::Generic);
    }
    for (const std::uint32_t id : result.enemies.walking) {
        RemoveEnemy(id, Enemy::Walking);
    }
    for (const std::uint32_t id : result.enemies.flying) {
        RemoveEnemy(id, Enemy::Flying);
    }
    for (const std::uint32_t id : result.missiles.player) {
        RemoveMissile(id, Missile::Player);
    }
    for (const std::uint32_t id : result.missiles.force) {
        RemoveMissile(id, Missile::Force);
    }
    for (const std::uint32_t id : result.missiles.enemy) {
        RemoveMissile(id, Missile::Enemy);
    }
    for (const std::uint32_t id : result.missiles.boss) {
        RemoveMissile(id, Missile::Boss);
    }
    for (const auto& [shieldId, playerId] : result.shields) {
        RemoveItem(shieldId, Item::Shield);
        SetPlayerIdStatistic(playerId, Statistic::Shield, true);
    }
    for (const auto& [forceId, playerId] : result.forces) {
        RemoveItem(forceId, Item::Force);
        SetPlayerIdStatistic(playerId, Statistic::Force, true);
    }
    for (const std::uint32_t id : result.players) {
        KillPlayerId(id);
    }
}

std::unordered_map<std::uint32_t, Entity>& Engine::Game::GetItems(const Item type)
{
    switch (type) {
        case Item::Shield:
            return _items.shield;
        case Item::Force:
            return _items.force;
        default:
            throw Exception::GenericError(std::format("Invalid item type, got {}", Misc::Utils::GetEnumIndex(type)));
    }
}

std::optional<std::reference_wrapper<Entity>> Engine::Game::GetItem(const std::uint32_t id, const Item type)
{
    std::unordered_map<std::uint32_t, Entity>& items = GetItems(type);
    auto it = items.find(id);

    if (it != items.end()) {
        return std::ref(it->second);
    }
    return std::nullopt;
}

std::uint32_t Engine::Game::CreateItem(const Position position, const Item type)
{
    Entity item = { .position = position, .id = 0, .health = 0 };

    switch (type) {
        case Item::Shield:
            item = { position, Misc::Utils::GetNextId(std::format("game-{}-shield-item", _id)), 0 };
            _items.shield[item.id] = item;
            break;
        case Item::Force:
            item = { position, Misc::Utils::GetNextId(std::format("game-{}-force-item", _id)), 0 };
            _items.force[item.id] = item;
            break;
        default:
            break;
    }
    for (const std::uint32_t& current : _ids) {
        if (current != 0) {
            Action::Dispatcher::SendMessage(ActionType::SPW, current, std::make_tuple(item.id, Misc::Utils::GetEnumIndex(type), item.position));
        }
    }
    return item.id;
}

void Engine::Game::RemoveItem(const std::uint32_t id, const Item type)
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

void Engine::Game::SetPlayerIdStatistic(const std::uint32_t id, const Statistic& statistic, const bool status)
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

void Engine::Game::SetPlayerIdStatistic(const std::shared_ptr<Network::Player>& player, const Statistic& statistic, const bool status, const bool god)
{
    if (player) {
        player->SetStatistic(statistic, status, god);
        for (const std::uint32_t& current : _ids) {
            if (current != 0) {
                Action::Dispatcher::SendMessage(ActionType::STS, current, std::make_tuple(player->GetId(), statistic, status));
            }
        }
    }
}

const std::string Engine::Game::MissileToString(const Missile type) const
{
    switch (type) {
        case Missile::Player:
            return "player";
        case Missile::Enemy:
            return "enemy";
        case Missile::Force:
            return "force";
        default:
            return "unknown";
    }
}

const std::string Engine::Game::EnemyToString(const Enemy type) const
{
    switch (type) {
        case Enemy::Generic:
            return "generic";
        case Enemy::Walking:
            return "walking";
        case Enemy::Flying:
            return "flying";
        default:
            return "unknown";
    }
}

const std::string Engine::Game::ItemToString(const Item type) const
{
    switch (type) {
        case Item::Shield:
            return "shield";
        case Item::Force:
            return "force";
        default:
            return "unknown";
    }
}

const std::string Engine::Game::CharacterToString(const Character type) const
{
    switch (type) {
        case Character::Player:
            return "player";
        default:
            return "unknown";
    }
}
