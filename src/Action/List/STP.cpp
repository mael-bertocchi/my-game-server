/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** STP.cpp
*/

#include "Miscellaneous/Logger.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Exception/Generic.hpp"
#include "Action/List/STP.hpp"
#include "Storage/Player.hpp"

void Action::List::STP::SendMessage(const std::uint32_t id, const std::any& content) const
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::Connected | PlayerValidation::Playing);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        }

        const Result result = std::any_cast<Result>(content);
        const std::vector<std::uint8_t> serialized = Misc::Utils::Serialize(result);

        player->PushMessage(Wrapper::Socket::Protocol::TCP, {
            .type = ActionType::STP,
            .body = serialized
        });
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to send STP to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to send STP to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to send STP to player {}", id), Misc::Logger::LogLevel::Critical);
    }
}

const std::string Action::List::STP::ResultToString(const Result result)
{
    switch (result) {
        case Result::Lose:
            return "lose";
        case Result::Win:
            return "win";
        default:
            throw "unknown";
    }
}
