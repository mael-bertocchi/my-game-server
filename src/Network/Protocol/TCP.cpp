/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCP.cpp
*/

#include "Network/Protocol/TCP.hpp"
#include "Miscellaneous/Logger.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Action/Dispatcher.hpp"
#include "Storage/Player.hpp"

#include <format>

std::vector<std::uint8_t> Network::Protocol::TCP::SerializeHeader(const Header& header)
{
    std::vector<std::uint8_t> content = {};

    content.resize(TCP_HEADER_SIZE);

    std::memcpy(content.data(), &header.type, HEADER_TYPE_SIZE);
    std::memcpy(content.data() + HEADER_TYPE_SIZE, &header.length, HEADER_LENGTH_SIZE);

    return content;
}

bool Network::Protocol::TCP::ReceiveMessage(const Wrapper::Socket::SocketType socket)
{
    std::vector<std::uint8_t> content = Wrapper::Socket::Receive(socket, TCP_HEADER_SIZE);

    if (content.empty()) {
        return true;
    } else if (content.size() < TCP_HEADER_SIZE) {
        Misc::Logger::Log(std::format("Failed to read header from player {}: incomplete header", socket), Misc::Logger::LogLevel::Caution);
        return false;
    }

    Header header = ParseHeader(content);
    std::vector<std::uint8_t> body = Wrapper::Socket::Receive(socket, header.length);

    if (body.size() != header.length) {
        Misc::Logger::Log(std::format("Failed to read body from player {}: expected {} bytes, got {}", socket, header.length, body.size()), Misc::Logger::LogLevel::Caution);
        return false;
    }

    std::optional<std::uint32_t> id = Storage::Cache::Player::GetInstance().GetPlayerIdBySocket(socket);

    if (id.has_value()) {
        Misc::Logger::Log(std::format("Received message from player {}: {} {}", id.value(), Misc::Utils::BytesToHex(content), Misc::Utils::BytesToHex(body)));
        Action::Dispatcher::ReceiveMessage(header.type, id.value(), body);
        return false;
    } else {
        Misc::Logger::Log(std::format("Received message from unknown socket {}, skipping", socket), Misc::Logger::LogLevel::Caution);
        return true;
    }
}

void Network::Protocol::TCP::SendMessage(const Wrapper::Socket::SocketType socket)
{
    const std::optional<std::uint32_t> id = Storage::Cache::Player::GetInstance().GetPlayerIdBySocket(socket);

    if (id.has_value()) {
        const std::shared_ptr<Network::Player>& player = Storage::Cache::Player::GetInstance().GetPlayerById(id.value());

        if (player) {
            while (player->HasMessage(Wrapper::Socket::Protocol::TCP)) {
                const Network::Player::Message message = player->PopMessage(Wrapper::Socket::Protocol::TCP);

                const std::uint32_t size = static_cast<std::uint32_t>(message.body.size());
                const std::vector<std::uint8_t> header = SerializeHeader({
                    .type = message.type,
                    .length = size
                });
                std::vector<std::uint8_t> serialized = {};

                serialized.reserve(header.size() + size);
                serialized.insert(serialized.end(), header.begin(), header.end());
                serialized.insert(serialized.end(), message.body.begin(), message.body.end());

                if (Wrapper::Socket::Send(socket, serialized) < 0) {
                    Misc::Logger::Log(std::format("Failed to send message to player {}: {}", id.value(), Wrapper::Socket::GetLastError()), Misc::Logger::LogLevel::Critical);
                } else {
                    Misc::Logger::Log(std::format("Sent message to player {}: {} {}", id.value(), Misc::Utils::BytesToHex(header), Misc::Utils::BytesToHex(message.body)));
                }
            }
        }
    }
}

Network::Protocol::TCP::Header Network::Protocol::TCP::ParseHeader(const std::vector<std::uint8_t>& content)
{
    Header header = { .type = ActionType::UKN, .length = 0 };

    if (content.size() < TCP_HEADER_SIZE) {
        return header;
    }

    std::memcpy(&header.type, content.data(), HEADER_TYPE_SIZE);
    std::memcpy(&header.length, content.data() + HEADER_TYPE_SIZE, HEADER_LENGTH_SIZE);

    return header;
}
