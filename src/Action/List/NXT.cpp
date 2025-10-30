/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NXT.cpp
*/

#include "Miscellaneous/Logger.hpp"
#include "Exception/Generic.hpp"
#include "Action/List/NXT.hpp"
#include "Storage/Player.hpp"

#include <format>

void Action::List::NXT::SendMessage(const std::uint32_t id, const std::any&) const
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::Connected | PlayerValidation::Playing);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        }

        player->PushMessage(Wrapper::Socket::Protocol::TCP, {
            .type = ActionType::NXT,
            .body = {}
        });
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to send NXT to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to send NXT to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to send NXT to player {}", id), Misc::Logger::LogLevel::Critical);
    }
}
