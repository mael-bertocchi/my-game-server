/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** JON.cpp
*/

#include "Miscellaneous/Logger.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Exception/Generic.hpp"
#include "Action/List/JON.hpp"
#include "Storage/Player.hpp"
#include "Storage/Game.hpp"

#include <format>

void Action::List::JON::ReceiveMessage(const std::uint32_t id, const std::vector<std::uint8_t>& body)
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        const std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::Connected | PlayerValidation::NotPlaying);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        } else if (body.size() != 4) {
            throw Exception::GenericError(std::format("Expected 4 bytes in the body, got {}", body.size()));
        }

        const std::uint32_t gameId = Misc::Utils::Deserialize<std::uint32_t>(body);
        const std::shared_ptr<Engine::Game>& game = Storage::Cache::Game::GetInstance().GetGameById(gameId);
        const std::optional<std::string> gameValidation = ValidateGame(game, GameValidation::NotFull | GameValidation::NotStarted);

        if (gameValidation.has_value()) {
            throw Exception::GenericError(gameValidation.value());
        }

        if (game->AddPlayerId(id)) {
            Storage::Cache::Game::GetInstance().AddPlayerToGame(id, gameId);
        }
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to process JON from player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to process JON from player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to process JON from player {}", id), Misc::Logger::LogLevel::Critical);
    }
}

void Action::List::JON::SendMessage(const std::uint32_t id, const std::any& content) const
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        const std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::Connected | PlayerValidation::Playing);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        }

        const std::vector<std::uint8_t> serialized = Misc::Utils::Serialize(std::any_cast<std::uint32_t>(content));

        player->PushMessage(Wrapper::Socket::Protocol::TCP, {
            .type = ActionType::JON,
            .body = serialized
        });
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to send JON to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to send JON to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to send JON to player {}", id), Misc::Logger::LogLevel::Critical);
    }
}
