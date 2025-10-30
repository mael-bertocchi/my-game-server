/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** STR.cpp
*/

#include "Miscellaneous/Logger.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Exception/Generic.hpp"
#include "Action/List/STR.hpp"
#include "Storage/Player.hpp"
#include "Storage/Game.hpp"

void Action::List::STR::ReceiveMessage(const std::uint32_t id, const std::vector<std::uint8_t>&)
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        const std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::Connected | PlayerValidation::Playing);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        }

        const std::shared_ptr<Engine::Game>& game = Storage::Cache::Game::GetInstance().GetGameByPlayerId(id);
        const std::optional<std::string> gameValidation = ValidateGame(game, GameValidation::NotStarted);

        if (gameValidation.has_value()) {
            throw Exception::GenericError(gameValidation.value());
        }

        game->Start();
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to process STR from player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to process STR from player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to process STR from player {}", id), Misc::Logger::LogLevel::Critical);
    }
}

void Action::List::STR::SendMessage(const std::uint32_t id, const std::any& content) const
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        const std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::Connected | PlayerValidation::Playing);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        }

        const auto& positions = std::any_cast<const std::vector<std::pair<std::uint32_t, Position>>&>(content);
        std::vector<std::uint8_t> serialized = {};

        serialized.push_back(static_cast<std::uint8_t>(positions.size()));

        for (const auto& [playerId, position] : positions) {
            const std::vector<std::uint8_t> data = Misc::Utils::Serialize(playerId, position.x, position.y);
            serialized.insert(serialized.end(), data.begin(), data.end());
        }

        player->PushMessage(Wrapper::Socket::Protocol::TCP, {
            .type = ActionType::STR,
            .body = serialized
        });
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to send STR to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to send STR to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to send STR to player {}", id), Misc::Logger::LogLevel::Critical);
    }
}
