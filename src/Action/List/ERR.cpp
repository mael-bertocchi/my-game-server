/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ERR.cpp
*/

#include "Miscellaneous/Logger.hpp"
#include "Exception/Generic.hpp"
#include "Action/List/ERR.hpp"
#include "Storage/Player.hpp"

void Action::List::ERR::SendMessage(const std::uint32_t id, const std::any& content) const
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        const std::optional<std::string> playerValidation = ValidatePlayer(player);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        }

        const std::string message = std::any_cast<std::string>(content);
        const std::vector<std::uint8_t> serialized(message.begin(), message.end());

        player->PushMessage(Wrapper::Socket::Protocol::TCP, {
            .type = ActionType::ERR,
            .body = serialized
        });
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to send ERR to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to send ERR to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to send ERR to player {}", id), Misc::Logger::LogLevel::Critical);
    }
}
