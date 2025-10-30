/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** UDP.cpp
*/

#include "Miscellaneous/Logger.hpp"
#include "Network/Protocol/UDP.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Action/Dispatcher.hpp"
#include "Network/Player.hpp"
#include "Storage/Player.hpp"

#include <format>

Network::Protocol::UDP::UDP(const Wrapper::Socket::SocketType socket) : _socket(socket) {}

void Network::Protocol::UDP::ReceiveMessage()
{
    std::string address = "";
    std::uint16_t port = 0;

    const std::vector<std::uint8_t> content = Wrapper::Socket::ReceiveFrom(_socket, address, port);
    const std::size_t size = content.size();

    if (size > 0) {
        const ActionType type = static_cast<ActionType>(content[0]);
        std::vector<std::uint8_t> payload = {};

        if (size > 1) {
            payload.assign(content.begin() + 1, content.end());
        }

        const std::shared_ptr<Network::Player> player = Storage::Cache::Player::GetInstance().GetPlayerByAddress(address, port);

        if (player) {
            Action::Dispatcher::ReceiveMessage(type, player->GetId(), payload);
            Misc::Logger::Log(std::format("Received message from player {}: {}", player->GetId(), Misc::Utils::BytesToHex(payload)), Misc::Logger::LogLevel::Network);
        }
    }
}

void Network::Protocol::UDP::SendMessage(const Wrapper::Socket::SocketType socket)
{
    const std::shared_ptr<Network::Player> player = Storage::Cache::Player::GetInstance().GetPlayerBySocket(socket);

    if (player) {
        const std::string& address = player->GetAddress();
        const std::uint16_t port = player->GetPort();

        while (player->HasMessage(Wrapper::Socket::Protocol::UDP)) {
            const Network::Player::Message message = player->PopMessage(Wrapper::Socket::Protocol::UDP);

            std::vector<std::uint8_t> content = {};
            content.reserve(1 + message.body.size());

            content.push_back(static_cast<std::uint8_t>(message.type));

            if (!message.body.empty()) {
                content.insert(content.end(), message.body.begin(), message.body.end());
            }

            if (Wrapper::Socket::SendTo(_socket, content, address, port) < 0) {
                Misc::Logger::Log(std::format("Failed to send message to player {}: {}", player->GetId(), Wrapper::Socket::GetLastError()), Misc::Logger::LogLevel::Critical);
            } else {
                Misc::Logger::Log(std::format("Sent message to player {}: {}", player->GetId(), Misc::Utils::BytesToHex(content)), Misc::Logger::LogLevel::Network);
            }
        }
    }
}
