/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** OVE.cpp
*/

#include "Miscellaneous/Logger.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Exception/Generic.hpp"
#include "Action/List/OVE.hpp"
#include "Storage/Player.hpp"
#include "Storage/Game.hpp"
#include "Types.hpp"

#include <format>

void Action::List::OVE::ReceiveMessage(const std::uint32_t id, const std::vector<std::uint8_t>& body)
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        const std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::Connected | PlayerValidation::Playing);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        } else if (body.size() != 1) {
            throw Exception::GenericError(std::format("Expected body size 1, got {}", body.size()));
        }

        const std::shared_ptr<Engine::Game>& game = Storage::Cache::Game::GetInstance().GetGameByPlayerId(id);
        const std::optional<std::string> gameValidation = ValidateGame(game, GameValidation::Started);

        if (gameValidation.has_value()) {
            throw Exception::GenericError(gameValidation.value());
        }

        const Direction direction = static_cast<Direction>(body[0]);
        const Position position = player->Move(direction);

        game->QueuePosition(id, Misc::Utils::GetEnumIndex(CharacterType::Player), position);
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to process OVE for player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to process OVE for player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to process OVE for player {}", id), Misc::Logger::LogLevel::Critical);
    }
}
