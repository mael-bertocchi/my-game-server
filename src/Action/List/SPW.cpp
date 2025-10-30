/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SPW.cpp
*/

#include "Miscellaneous/Logger.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Action/List/SPW.hpp"
#include "Storage/Player.hpp"
#include "Types.hpp"
#include "Variables.hpp"

#include <format>

void Action::List::SPW::SendMessage(const std::uint32_t id, const std::any& content) const
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        const std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::Connected | PlayerValidation::Playing);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        }

        const auto& [entityId, entityType, position] = std::any_cast<std::tuple<std::uint32_t, std::uint8_t, Position>>(content);
        if (entityType > MAX_SPAWNABLE_ENTITY_VALUE) {
            throw Exception::GenericError(std::format("Entity type out of range, got {}", entityType));
        }
        const std::vector<std::uint8_t> serialized = Misc::Utils::Serialize(entityId, entityType, position);

        player->PushMessage(Wrapper::Socket::Protocol::UDP, {
            .type = ActionType::SPW,
            .body = serialized
        });
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to send SPW to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to send SPW to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to send SPW to player {}", id), Misc::Logger::LogLevel::Critical);
    }
}
