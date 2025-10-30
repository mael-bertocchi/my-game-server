/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** DIE.cpp
*/

#include "Miscellaneous/Logger.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Exception/Generic.hpp"
#include "Action/List/DIE.hpp"
#include "Storage/Player.hpp"
#include "Variables.hpp"

#include <cstdint>
#include <format>

void Action::List::DIE::SendMessage(const std::uint32_t id, const std::any& content) const
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        const std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::Connected | PlayerValidation::Playing);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        }

        const auto& [entityId, entityType] = std::any_cast<std::pair<std::uint32_t, std::uint8_t>>(content);
        if (entityType > MAX_ENTITY_VALUE) {
            throw Exception::GenericError(std::format("Entity type is out of range, got {}", entityType));
        }
        const std::vector<std::uint8_t> serialized = Misc::Utils::Serialize(entityId, entityType);

        player->PushMessage(Wrapper::Socket::Protocol::UDP, {
            .type = ActionType::DIE,
            .body = serialized
        });
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to send DIE to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to send DIE to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to send DIE to player {}", id), Misc::Logger::LogLevel::Critical);
    }
}
