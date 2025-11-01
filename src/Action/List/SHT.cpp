/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SHT.cpp
*/

#include "Miscellaneous/Logger.hpp"
#include "Exception/Generic.hpp"
#include "Action/List/SHT.hpp"
#include "Network/Player.hpp"
#include "Storage/Player.hpp"
#include "Storage/Game.hpp"
#include "Types.hpp"

#include <format>

void Action::List::SHT::ReceiveMessage(const std::uint32_t id, const std::vector<std::uint8_t>&)
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        const std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::Connected | PlayerValidation::Playing);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        }

        const std::shared_ptr<Engine::Game>& game = Storage::Cache::Game::GetInstance().GetGameByPlayerId(id);
        const std::optional<std::string> gameValidation = ValidateGame(game, GameValidation::Started);

        if (gameValidation.has_value()) {
            throw Exception::GenericError(gameValidation.value());
        }

        const Position& position = player->GetPosition();
        if (player->IsStatisticActive(Statistic::Force)) {
            game->CreateMissile(Missile::Force, position);
        } else {
            game->CreateMissile(Missile::Player, position);
        }
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to process SHT for player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to process SHT for player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to process SHT for player {}", id), Misc::Logger::LogLevel::Critical);
    }
}
