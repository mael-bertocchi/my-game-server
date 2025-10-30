/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** STS.cpp
*/

#include "Miscellaneous/Logger.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Exception/Generic.hpp"
#include "Action/List/STS.hpp"
#include "Network/Player.hpp"
#include "Storage/Player.hpp"

#include <format>

void Action::List::STS::SendMessage(const std::uint32_t id, const std::any& content) const
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        const std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::Connected | PlayerValidation::Playing);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        }

        const auto& [playerId, statistic, status] = std::any_cast<std::tuple<std::uint32_t, Network::Player::Statistic, bool>>(content);
        const std::vector<std::uint8_t> serialized = Misc::Utils::Serialize(playerId, statistic, status);

        player->PushMessage(Wrapper::Socket::Protocol::UDP, {
            .type = ActionType::STS,
            .body = serialized
        });
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to process STS for player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to process STS for player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to process STS for player {}", id), Misc::Logger::LogLevel::Critical);
    }
}
