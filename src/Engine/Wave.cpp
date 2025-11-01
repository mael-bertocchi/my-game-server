/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Wave.cpp
*/

#include "Miscellaneous/Environment.hpp"
#include "Miscellaneous/Logger.hpp"
#include "Exception/Generic.hpp"
#include "Engine/Wave.hpp"
#include "Engine/Game.hpp"
#include "Storage/Game.hpp"
#include "Storage/Player.hpp"
#include "Variables.hpp"
#include "Types.hpp"

#include <optional>
#include <format>
#include <string>

Engine::Wave::Wave(const std::uint32_t id) : _next(0), _id(id)
{
    std::shared_ptr<Engine::Game> game = Storage::Cache::Game::GetInstance().GetGameById(id);

    if (!game) {
        throw Exception::GenericError(std::format("Cannot create wave for non-existing game {}", id));
    }

    _state.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string);
    _game = std::weak_ptr<void>(std::static_pointer_cast<void>(game));

    RegisterLuaBindings();
    SwitchToNextWave();
}

Engine::Wave::~Wave()
{
    if (_callback.valid()) {
        _callback.abandon();
    }
    _state.collect_garbage();
}

void Engine::Wave::RegisterLuaBindings()
{
    _state.new_usertype<Position>("Position", sol::call_constructor, sol::constructors<Position(), Position(std::uint16_t, std::uint16_t)>(), "x", &Position::x, "y", &Position::y, "new", sol::factories([](std::uint16_t x, std::uint16_t y) {
        return Position{x, y};
    }));

    _state.new_usertype<Entity>("Entity", "id", &Entity::id, "position", &Entity::position);

    _state.new_enum<Enemy>("EnemyType", {
        {"Generic", Enemy::Generic},
        {"Walking", Enemy::Walking},
        {"Flying", Enemy::Flying},
        {"Boss", Enemy::Boss}
    });

    _state.new_enum<Missile>("MissileType", {
        {"Player", Missile::Player},
        {"Enemy", Missile::Enemy},
        {"Boss", Missile::Boss}
    });

    _state.new_enum<Item>("ItemType", {
        {"Shield", Item::Shield},
        {"Force", Item::Force}
    });

    _state.set_function("GetEnemiesByType", [this](Enemy type) -> sol::table {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            const auto& enemies = game->GetEnemies(type);
            sol::table result = _state.create_table();
            std::size_t i = 1;

            for (const auto& [id, entity] : enemies) {
                result[i++] = entity;
            }
            return result;
        }
        return _state.create_table();
    });

    _state.set_function("GetEnemies", [this]() -> sol::table {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            sol::table result = _state.create_table();
            std::size_t i = 1;

            const auto& generic = game->GetEnemies(Enemy::Generic);
            for (const auto& [id, entity] : generic) {
                result[i++] = entity;
            }

            const auto& walking = game->GetEnemies(Enemy::Walking);
            for (const auto& [id, entity] : walking) {
                result[i++] = entity;
            }

            const auto& flying = game->GetEnemies(Enemy::Flying);
            for (const auto& [id, entity] : flying) {
                result[i++] = entity;
            }

            const auto& boss = game->GetEnemies(Enemy::Boss);
            for (const auto& [id, entity] : boss) {
                result[i++] = entity;
            }
            return result;
        }
        return _state.create_table();
    });

    _state.set_function("CreateEnemy", [this](Enemy type, Position pos) -> std::uint32_t {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            return game->CreateEnemy(type, pos);
        }
        return 0;
    });

    _state.set_function("MoveEnemy", [this](std::uint32_t id, std::int16_t dx, std::int16_t dy, Enemy type) {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            game->MoveEnemy(id, type, dx, dy);
        }
    });

    _state.set_function("RemoveEnemy", [this](std::uint32_t id, Enemy type) {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            game->RemoveEnemy(id, type);
        }
    });

    _state.set_function("GetMissilesByType", [this](Missile type) -> sol::table {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            const auto& missiles = game->GetMissiles(type);
            sol::table result = _state.create_table();
            std::size_t i = 1;

            for (const auto& [id, entity] : missiles) {
                result[i++] = entity;
            }
            return result;
        }
        return _state.create_table();
    });

    _state.set_function("CreateMissile", [this](Missile type, Position pos) -> std::uint32_t {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            return game->CreateMissile(type, pos);
        }
        return 0;
    });

    _state.set_function("MoveMissile", [this](std::uint32_t id, Missile type, std::int16_t dx, std::int16_t dy) {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            game->MoveMissile(id, type, dx, dy);
        }
    });

    _state.set_function("RemoveMissile", [this](std::uint32_t id, Missile type) {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            game->RemoveMissile(id, type);
        }
    });

    _state.set_function("GetItemsByType", [this](Item type) -> sol::table {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            const auto& items = game->GetItems(type);
            sol::table result = _state.create_table();
            std::size_t i = 1;

            for (const auto& [id, entity] : items) {
                result[i++] = entity;
            }
            return result;
        }
        return _state.create_table();
    });

    _state.set_function("GetItems", [this]() -> sol::table {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            sol::table result = _state.create_table();
            std::size_t i = 1;

            const auto& shields = game->GetItems(Item::Shield);
            for (const auto& [id, entity] : shields) {
                result[i++] = entity;
            }

            const auto& forces = game->GetItems(Item::Force);
            for (const auto& [id, entity] : forces) {
                result[i++] = entity;
            }

            return result;
        }
        return _state.create_table();
    });

    _state.set_function("CreateItem", [this](Position pos, Item type) -> std::uint32_t {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            return game->CreateItem(pos, type);
        }
        return 0;
    });

    _state.set_function("RemoveItem", [this](std::uint32_t id, Item type) {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            game->RemoveItem(id, type);
        }
    });

    _state.set_function("GetWidth", []() -> std::uint16_t {
        return WINDOW_WIDTH;
    });

    _state.set_function("GetHeight", []() -> std::uint16_t {
        return WINDOW_HEIGHT;
    });

    _state.set_function("GetGameId", [this]() -> std::uint32_t {
        return _id;
    });

    _state.set_function("GetPlayerCount", [this]() -> std::uint8_t {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            return game->GetPlayerCount();
        }
        return 0;
    });

    _state.set_function("GetPlayerIds", [this]() -> sol::table {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            const auto& ids = game->GetPlayerIds();
            sol::table result = _state.create_table();
            std::size_t i = 1;

            for (const auto& id : ids) {
                if (id != 0) {
                    result[i++] = id;
                }
            }
            return result;
        }
        return _state.create_table();
    });

    _state.set_function("GetPlayerPosition", [](std::uint32_t playerId) -> sol::optional<Position> {
        std::shared_ptr<Network::Player> player = Storage::Cache::Player::GetInstance().GetPlayerById(playerId);
        if (player) {
            return player->GetPosition();
        }
        return sol::nullopt;
    });

    _state.set_function("GetPlayers", [this]() -> sol::table {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            const auto& ids = game->GetPlayerIds();
            sol::table result = _state.create_table();
            std::size_t i = 1;

            for (const auto& id : ids) {
                if (id != 0) {
                    std::shared_ptr<Network::Player> player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
                    if (player) {
                        sol::table data = _state.create_table();
                        data["id"] = id;
                        data["position"] = player->GetPosition();
                        result[i++] = data;
                    }
                }
            }
            return result;
        }
        return _state.create_table();
    });

    _state.set_function("Log", [this](const std::string& message) {
        Misc::Logger::Log(std::format("[Game - {}] {}", _id, message));
    });

    _state.set_function("Random", [](std::int32_t min, std::int32_t max) -> std::int32_t {
        return min + (std::rand() % (max - min + 1));
    });
}

Engine::Wave::Result Engine::Wave::Process(float deltaTime)
{
    try {
        sol::protected_function_result result = _callback(deltaTime);

        if (!result.valid()) {
            sol::error err = result;
            Misc::Logger::Log(std::format("[Game - {}] Failed to process wave: {}", _id, err.what()), Misc::Logger::LogLevel::Critical);
            return SwitchToNextWave();
        }
        if (result.return_count() > 0) {
            sol::optional<bool> complete = result;
            if (complete.value_or(false)) {
                Misc::Logger::Log(std::format("[Game - {}] Wave completed", _id));
                return SwitchToNextWave();
            }
        }
        return Result::Continue;
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("[Game - {}] Failed to process wave: {}", _id, ex.what()), Misc::Logger::LogLevel::Critical);
        return SwitchToNextWave();
    } catch (...) {
        Misc::Logger::Log(std::format("[Game - {}] Failed to process wave: Unknown error", _id), Misc::Logger::LogLevel::Critical);
        return SwitchToNextWave();
    }
}

std::optional<std::string> Engine::Wave::GetNextWaveScriptPath() const
{
    const auto& waves = Misc::Env::GetInstance().GetConfiguration<Misc::Env::Waves>();

    if (_next >= waves.files.size()) {
        return std::nullopt;
    }
    return waves.files[_next];
}

void Engine::Wave::LoadWaveScript(const std::string& filepath)
{
    sol::protected_function_result result = _state.script_file(filepath);
    if (!result.valid()) {
        sol::error err = result;
        throw Exception::GenericError(err.what());
    }
}

void Engine::Wave::LoadWaveCallback()
{
    sol::optional<sol::function> func = _state["Process"];
    if (func.has_value()) {
        _callback = func.value();
    } else {
        throw Exception::GenericError("Wave script does not contain the process function");
    }
}

void Engine::Wave::InitializeWave()
{
    sol::optional<sol::function> func = _state["OnInit"];
    if (func.has_value()) {
        sol::protected_function_result result = func.value()();
        if (!result.valid()) {
            sol::error err = result;
            throw Exception::GenericError(std::format("Failed to initialize: {}", err.what()));
        }
    }
}

Engine::Wave::Result Engine::Wave::SwitchToNextWave()
{
    const std::optional<std::string> filepath = GetNextWaveScriptPath();

    if (filepath.has_value()) {
        try {
            _callback.reset();

            LoadWaveScript(filepath.value());

            LoadWaveCallback();
            InitializeWave();

            Misc::Logger::Log(std::format("[Game - {}] Switched to wave {}", _id, _next++));
            return Result::Next;
        } catch (const std::exception& ex) {
            Misc::Logger::Log(std::format("[Game - {}] Failed to load wave: {}", _id, ex.what()), Misc::Logger::LogLevel::Critical);
        } catch (...) {
            Misc::Logger::Log(std::format("[Game - {}] Failed to load wave: Unknown error", _id), Misc::Logger::LogLevel::Critical);
        }
    } else {
        Misc::Logger::Log(std::format("[Game - {}] No more waves available", _id));
    }
    return Result::Stop;
}
