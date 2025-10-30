/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** POS.cpp
*/

#include "Miscellaneous/Logger.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Action/List/POS.hpp"
#include "Storage/Player.hpp"

#include <cstdint>
#include <format>

void Action::List::POS::SendMessage(const std::uint32_t id, const std::any& content) const
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        const std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::Connected | PlayerValidation::Playing);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        }

        const auto& positions = std::any_cast<const std::vector<std::tuple<std::uint32_t, std::uint8_t, Position>>&>(content);
        std::vector<std::uint8_t> serialized = Misc::Utils::Serialize<std::uint16_t>(static_cast<std::uint16_t>(positions.size()));

        for (const auto& [entityId, entityType, position] : positions) {
            if (entityType > MAX_ENTITY_VALUE) {
                throw Exception::GenericError(std::format("Entity type is out of range, got {}", entityType));
            }
            std::vector<std::uint8_t> update = Misc::Utils::Serialize(entityId, entityType, position);
            serialized.insert(serialized.end(), update.begin(), update.end());
        }

        player->PushMessage(Wrapper::Socket::Protocol::UDP, {
            .type = ActionType::POS,
            .body = serialized
        });
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to send POS for player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to send POS for player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to send POS for player {}", id), Misc::Logger::LogLevel::Critical);
    }
}
