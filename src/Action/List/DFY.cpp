/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** DFY.cpp
*/

#include "Miscellaneous/Logger.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Exception/Generic.hpp"
#include "Action/Dispatcher.hpp"
#include "Action/List/DFY.hpp"
#include "Storage/Player.hpp"

#include <format>

void Action::List::DFY::ReceiveMessage(const std::uint32_t id, const std::vector<std::uint8_t>& body)
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        const std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::NotConnected | PlayerValidation::NotPlaying);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        } else if (body.empty()) {
            throw Exception::GenericError("Body must contain username and password separated by space");
        }

        const std::string content(body.begin(), body.end());
        const std::vector<std::string> parts = Misc::Utils::Split(content);

        if (parts.size() != 2) {
            throw Exception::GenericError(std::format("Expected username and password separated by space, got {}", parts.size()));
        }

        if (player->DoesExist(parts[0])) {
            if (player->Connect(parts[0], parts[1])) {
                Dispatcher::SendMessage(ActionType::DFY, id, State::ValidCredentials);
            } else {
                Dispatcher::SendMessage(ActionType::DFY, id, State::InvalidCredentials);
            }
        } else {
            if (player->Register(parts[0], parts[1])) {
                Dispatcher::SendMessage(ActionType::DFY, id, State::ValidCredentials);
            } else {
                Dispatcher::SendMessage(ActionType::DFY, id, State::InvalidCredentials);
            }
        }
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to process DFY from player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to process DFY from player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to process DFY from player {}", id), Misc::Logger::LogLevel::Critical);
    }
}

void Action::List::DFY::SendMessage(const std::uint32_t id, const std::any& content) const
{
    try {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id);
        const std::optional<std::string> playerValidation = ValidatePlayer(player, PlayerValidation::NotPlaying);

        if (playerValidation.has_value()) {
            throw Exception::GenericError(playerValidation.value());
        }

        const State state = std::any_cast<State>(content);
        std::vector<std::uint8_t> serialized = {};

        if (state != State::ValidCredentials) {
            serialized = Misc::Utils::Serialize(state);
        } else {
            serialized = Misc::Utils::Serialize(state, id);
        }

        player->PushMessage(Wrapper::Socket::Protocol::TCP, {
            .type = ActionType::DFY,
            .body = serialized
        });
    } catch (const Exception::GenericError& ex) {
        Misc::Logger::Log(std::format("Failed to send DFY to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (const std::exception& ex) {
        Misc::Logger::Log(std::format("Failed to send DFY to player {}: {}", id, ex.what()), Misc::Logger::LogLevel::Critical);
    } catch (...) {
        Misc::Logger::Log(std::format("Failed to send DFY to player {}", id), Misc::Logger::LogLevel::Critical);
    }
}
