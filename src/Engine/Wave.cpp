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
#include "Variables.hpp"
#include "Types.hpp"

#include <optional>
#include <format>

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

    _state.new_enum<EnemyType>("EnemyType", {
        {"Generic", EnemyType::Generic},
        {"Walking", EnemyType::Walking},
        {"Flying", EnemyType::Flying}
    });

    _state.new_enum<MissileType>("MissileType", {
        {"Player", MissileType::Player},
        {"Enemy", MissileType::Enemy}
    });

    _state.new_enum<ItemType>("ItemType", {
        {"Shield", ItemType::Shield},
        {"Force", ItemType::Force}
    });

    _state.set_function("GetEnemiesByType", [this](EnemyType type) -> sol::table {
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

            const auto& generic = game->GetEnemies(EnemyType::Generic);
            for (const auto& [id, entity] : generic) {
                result[i++] = entity;
            }

            const auto& walking = game->GetEnemies(EnemyType::Walking);
            for (const auto& [id, entity] : walking) {
                result[i++] = entity;
            }

            const auto& flying = game->GetEnemies(EnemyType::Flying);
            for (const auto& [id, entity] : flying) {
                result[i++] = entity;
            }
            return result;
        }
        return _state.create_table();
    });

    _state.set_function("CreateEnemy", [this](Position pos, sol::optional<EnemyType> type) -> std::uint32_t {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            return game->CreateEnemy(pos, type.value_or(EnemyType::Generic));
        }
        return 0;
    });

    _state.set_function("MoveEnemy", [this](std::uint32_t id, std::int16_t dx, std::int16_t dy, EnemyType type) {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            game->MoveEnemy(id, type, dx, dy);
        }
    });

    _state.set_function("RemoveEnemy", [this](std::uint32_t id, EnemyType type) {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            game->RemoveEnemy(id, type);
        }
    });

    _state.set_function("GetMissilesByType", [this](MissileType type) -> sol::table {
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

    _state.set_function("GetPlayerMissiles", [this]() -> sol::table {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            const auto& missiles = game->GetMissiles(MissileType::Player);
            sol::table result = _state.create_table();
            std::size_t i = 1;

            for (const auto& [id, entity] : missiles) {
                result[i++] = entity;
            }
            return result;
        }
        return _state.create_table();
    });

    _state.set_function("GetEnemyMissiles", [this]() -> sol::table {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            const auto& missiles = game->GetMissiles(MissileType::Enemy);
            sol::table result = _state.create_table();
            std::size_t i = 1;

            for (const auto& [id, entity] : missiles) {
                result[i++] = entity;
            }
            return result;
        }
        return _state.create_table();
    });

    _state.set_function("CreatePlayerMissile", [this](Position pos) -> std::uint32_t {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            return game->CreateMissile(pos, MissileType::Player);
        }
        return 0;
    });

    _state.set_function("CreateEnemyMissile", [this](Position pos) -> std::uint32_t {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            return game->CreateMissile(pos, MissileType::Enemy);
        }
        return 0;
    });

    _state.set_function("CreateMissile", [this](Position pos, MissileType type) -> std::uint32_t {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            return game->CreateMissile(pos, type);
        }
        return 0;
    });

    _state.set_function("MoveEnemyMissile", [this](std::uint32_t id, std::int16_t dx, std::int16_t dy) {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            game->MoveMissile(id, dx, dy);
        }
    });

    _state.set_function("RemoveMissile", [this](std::uint32_t id, MissileType type) {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            game->RemoveMissile(id, type);
        }
    });

    _state.set_function("GetItemsByType", [this](ItemType type) -> sol::table {
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

            const auto& shields = game->GetItems(ItemType::Shield);
            for (const auto& [id, entity] : shields) {
                result[i++] = entity;
            }

            const auto& forces = game->GetItems(ItemType::Force);
            for (const auto& [id, entity] : forces) {
                result[i++] = entity;
            }

            return result;
        }
        return _state.create_table();
    });

    _state.set_function("CreateItem", [this](Position pos, ItemType type) -> std::uint32_t {
        auto ptr = _game.lock();
        if (ptr) {
            const auto& game = std::static_pointer_cast<Engine::Game>(ptr);
            return game->CreateItem(pos, type);
        }
        return 0;
    });

    _state.set_function("RemoveItem", [this](std::uint32_t id, ItemType type) {
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

    _state.set_function("Log", [](const std::string& message) {
        Misc::Logger::Log(message);
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
            Misc::Logger::Log(std::format("Error in wave process function for game {}: {}", _id, err.what()), Misc::Logger::LogLevel::Critical);
            return SwitchToNextWave();
        }
        if (result.return_count() > 0) {
            sol::optional<bool> complete = result;
            if (complete.value_or(false)) {
                Misc::Logger::Log(std::format("Wave completed for game {}, switching to next one", _id));
                return SwitchToNextWave();
            }
        }
        return Result::Continue;
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Error while processing wave for game {}: {}", _id, ex.what()), Misc::Logger::LogLevel::Critical);
        return SwitchToNextWave();
    } catch (...) {
        Misc::Logger::Log(std::format("Unknown error while processing wave for game {}", _id), Misc::Logger::LogLevel::Critical);
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
        throw Exception::GenericError("Wave script does not contain the Process function");
    }
}

void Engine::Wave::InitializeWave()
{
    sol::optional<sol::function> func = _state["OnInit"];
    if (func.has_value()) {
        sol::protected_function_result result = func.value()();
        if (!result.valid()) {
            sol::error err = result;
            throw Exception::GenericError(std::format("OnInit failed: {}", err.what()));
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

            Misc::Logger::Log(std::format("Game {} switched to wave {} with {}", _id, _next++, filepath.value()));
            return Result::Next;
        } catch (const std::exception& ex) {
            Misc::Logger::Log(std::format("Failed to load wave for game {} from file {}: {}", _id, filepath.value(), ex.what()), Misc::Logger::LogLevel::Critical);
        } catch (...) {
            Misc::Logger::Log(std::format("Failed to load wave for game {} from file {}: Unknown error", _id, filepath.value()), Misc::Logger::LogLevel::Critical);
        }
    } else {
        Misc::Logger::Log(std::format("No more waves available for game {}", _id));
    }
    return Result::Stop;
}
