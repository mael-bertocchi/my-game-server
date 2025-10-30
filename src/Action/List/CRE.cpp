/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** CRE.cpp
*/

#include "Miscellaneous/Logger.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Exception/Generic.hpp"
#include "Action/Dispatcher.hpp"
#include "Action/List/CRE.hpp"
#include "Storage/Player.hpp"
#include "Storage/Game.hpp"

#include <format>

void Action::List::CRE::ReceiveMessage(const std::uint32_t id, const std::vector<std::uint8_t>&)
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        const std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::Connected | PlayerValidation::NotPlaying);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        }

        const auto game = std::make_shared<Engine::Game>();

        if (game) {
            Storage::Cache::Game::GetInstance().AddGame(game);
            Action::Dispatcher::SendMessage(ActionType::CRE, id, game->GetId());
        }
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to process CRE from player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to process CRE from player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to process CRE from player {}", id), Misc::Logger::LogLevel::Critical);
    }
}

void Action::List::CRE::SendMessage(const std::uint32_t id, const std::any& content) const
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        const std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::Connected | PlayerValidation::NotPlaying);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        }

        const std::uint32_t game = std::any_cast<std::uint32_t>(content);
        const std::vector<std::uint8_t> serialized = Misc::Utils::Serialize<std::uint32_t>(game);

        player->PushMessage(Wrapper::Socket::Protocol::TCP, {
            .type = ActionType::CRE,
            .body = serialized
        });
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to send CRE to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to send CRE to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to send CRE to player {}", id), Misc::Logger::LogLevel::Critical);
    }
}
