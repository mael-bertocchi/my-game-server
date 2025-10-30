/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LVE.cpp
*/

#include "Miscellaneous/Logger.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Exception/Generic.hpp"
#include "Action/List/LVE.hpp"
#include "Storage/Player.hpp"
#include "Storage/Game.hpp"

#include <format>

void Action::List::LVE::ReceiveMessage(const std::uint32_t id, const std::vector<std::uint8_t>&)
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::Connected | PlayerValidation::Playing);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        }

        const std::shared_ptr<Engine::Game>& game = Storage::Cache::Game::GetInstance().GetGameByPlayerId(id);
        std::optional<std::string> gameValidation = ValidateGame(game);

        if (gameValidation.has_value()) {
            throw Exception::GenericError(gameValidation.value());
        }

        if (game->RemovePlayerId(id)) {
            Storage::Cache::Game::GetInstance().RemovePlayerFromGame(id);
        }
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to process LVE from player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to process LVE from player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to process LVE from player {}", id), Misc::Logger::LogLevel::Critical);
    }
}

void Action::List::LVE::SendMessage(const std::uint32_t id, const std::any& content) const
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::Connected);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        }

        const std::uint32_t playerId = std::any_cast<std::uint32_t>(content);
        const std::vector<std::uint8_t> serialized = Misc::Utils::Serialize(playerId);

        player->PushMessage(Wrapper::Socket::Protocol::TCP, {
            .type = ActionType::LVE,
            .body = serialized
        });
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to send LVE to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to send LVE to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to send LVE to player {}", id), Misc::Logger::LogLevel::Critical);
    }
}
