/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Transceiver.cpp
*/

#include "Miscellaneous/Environment.hpp"
#include "Network/Protocol/TCP.hpp"
#include "Miscellaneous/Logger.hpp"
#include "Network/Transceiver.hpp"
#include "Exception/Generic.hpp"
#include "Action/Dispatcher.hpp"
#include "Action/List/DFY.hpp"
#include "Storage/Player.hpp"
#include "Storage/Game.hpp"
#include "Variables.hpp"

#include <format>
#include <thread>
#include <chrono>

Network::Transceiver::Transceiver()
{
    if (!Wrapper::Socket::Initialize()) {
        throw Exception::GenericError(std::format("Failed to initialize socket library: {}", Wrapper::Socket::GetLastError()));
    }

    _sockets = {
        .udp = CreateSocket(Wrapper::Socket::Protocol::UDP),
        .tcp = CreateSocket(Wrapper::Socket::Protocol::TCP)
    };

    _managers = {
        .udp = std::make_unique<Protocol::UDP>(_sockets.udp),
        .tcp = std::make_unique<Protocol::TCP>()
    };

    Misc::Logger::Log(std::format("Server started on port {}", Misc::Env::GetInstance().GetConfiguration<Misc::Env::Server>().port));

    _clients.push_back({
        .fd = _sockets.tcp,
        .events = POLLIN,
        .revents = 0
    });

    _clients.push_back({
        .fd = _sockets.udp,
        .events = POLLIN,
        .revents = 0
    });
}

Network::Transceiver::~Transceiver()
{
    Wrapper::Socket::Cleanup();

    Misc::Logger::Log("Server stopped");
}

void Network::Transceiver::SecureReceive(const std::atomic<bool>& running)
{
    while (running.load()) {
        try {
            std::vector<Wrapper::Socket::SocketType> disconnected = {};
            std::vector<Wrapper::Socket::SocketType> clients = {};
            std::vector<Wrapper::Socket::PollType> sockets = {};

            {
                std::shared_lock<std::shared_mutex> lock(_mutex);
                sockets.push_back({
                    .fd = _sockets.tcp,
                    .events = POLLIN,
                    .revents = 0
                });

                for (const auto& client : _clients) {
                    if (client.fd != _sockets.tcp && client.fd != _sockets.udp) {
                        sockets.push_back(client);
                    }
                }
            }

            if (sockets.empty()) {
                continue;
            }

            if (Wrapper::Socket::Poll(sockets, POLL_TIMEOUT_MS) < 0) {
                throw Exception::GenericError(std::format("Poll error: {}", Wrapper::Socket::GetLastError()));
            }

            for (const auto& socket : sockets) {
                if (socket.revents & (POLLHUP | POLLERR | POLLNVAL)) {
                    if (socket.fd != _sockets.tcp) {
                        disconnected.push_back(socket.fd);
                    }
                    continue;
                }

                if (socket.revents & POLLIN) {
                    if (socket.fd == _sockets.tcp) {
                        if (auto accepted = HandleConnection(); accepted.has_value()) {
                            clients.push_back(accepted.value());
                        }
                        continue;
                    }

                    std::optional<std::uint32_t> id = Storage::Cache::Player::GetInstance().GetPlayerIdBySocket(socket.fd);
                    if (!id.has_value()) {
                        Misc::Logger::Log(std::format("Unregistered socket {} received activity, cleaning up", socket.fd), Misc::Logger::LogLevel::Caution);
                        disconnected.push_back(socket.fd);
                        continue;
                    }

                    if (_managers.tcp->ReceiveMessage(socket.fd)) {
                        disconnected.push_back(socket.fd);
                    }
                }
            }

            if (!clients.empty()) {
                std::unique_lock<std::shared_mutex> lock(_mutex);
                for (const auto& fd : clients) {
                    _clients.push_back({
                        .fd = fd,
                        .events = POLLIN,
                        .revents = 0
                    });
                }
            }

            for (const auto& socket : disconnected) {
                HandleDisconnection(socket);
            }
        } catch (const std::exception& ex) {
            Misc::Logger::Log(std::format("Secure receive error: {}", ex.what()), Misc::Logger::LogLevel::Critical);
        } catch (...) {
            Misc::Logger::Log("Unknown secure receive error occurred", Misc::Logger::LogLevel::Critical);
        }
    }
}

void Network::Transceiver::SecureSend(const std::atomic<bool>& running)
{
    while (running.load()) {
        try {
            std::vector<Wrapper::Socket::SocketType> sockets = {};
            {
                std::shared_lock<std::shared_mutex> lock(_mutex);
                for (const auto& client : _clients) {
                    if (client.fd != _sockets.tcp && client.fd != _sockets.udp) {
                        sockets.push_back(client.fd);
                    }
                }
            }

            for (const auto& socket : sockets) {
                _managers.tcp->SendMessage(socket);
            }
        } catch (const std::exception& ex) {
            Misc::Logger::Log(std::format("Secure send error: {}", ex.what()), Misc::Logger::LogLevel::Critical);
        } catch (...) {
            Misc::Logger::Log("Unknown secure send error occurred", Misc::Logger::LogLevel::Critical);
        }
    }
}

void Network::Transceiver::Receive(const std::atomic<bool>& running)
{
    while (running.load()) {
        try {
            std::vector<Wrapper::Socket::PollType> sockets = {{
                .fd = _sockets.udp,
                .events = POLLIN,
                .revents = 0
            }};

            if (Wrapper::Socket::Poll(sockets, POLL_TIMEOUT_MS) < 0) {
                throw Exception::GenericError(std::format("Poll error: {}", Wrapper::Socket::GetLastError()));
            }

            bool activity = false;
            for (const auto& socket : sockets) {
                if (socket.revents & POLLIN) {
                    _managers.udp->ReceiveMessage();
                    activity = true;
                }
            }

            if (!activity) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        } catch (const std::exception& ex) {
            Misc::Logger::Log(std::format("Receive error: {}", ex.what()), Misc::Logger::LogLevel::Critical);
        } catch (...) {
            Misc::Logger::Log("Unknown receive error occurred", Misc::Logger::LogLevel::Critical);
        }
    }
}

void Network::Transceiver::Send(const std::atomic<bool>& running)
{
    while (running.load()) {
        try {
            std::vector<Wrapper::Socket::SocketType> sockets = {};
            {
                std::shared_lock<std::shared_mutex> lock(_mutex);
                for (const auto& client : _clients) {
                    if (client.fd != _sockets.tcp && client.fd != _sockets.udp) {
                        sockets.push_back(client.fd);
                    }
                }
            }

            for (const auto& socket : sockets) {
                _managers.udp->SendMessage(socket);
            }
        } catch (const std::exception& ex) {
            Misc::Logger::Log(std::format("Send error: {}", ex.what()), Misc::Logger::LogLevel::Critical);
        } catch (...) {
            Misc::Logger::Log("Unknown send error occurred", Misc::Logger::LogLevel::Critical);
        }
    }
}

std::optional<Wrapper::Socket::SocketType> Network::Transceiver::HandleConnection()
{
    std::pair<std::string, std::uint16_t> addr = {"", 0};
    std::optional<std::reference_wrapper<std::pair<std::string, std::uint16_t>>> opt(std::ref(addr));
    Wrapper::Socket::SocketType socket = Wrapper::Socket::Accept(_sockets.tcp, opt);

    if (!Wrapper::Socket::IsValid(socket)) {
        Misc::Logger::Log(std::format("Failed to accept new connection: {}", Wrapper::Socket::GetLastError()), Misc::Logger::LogLevel::Critical);
        return std::nullopt;
    }

    const std::shared_ptr<Network::Player> player = std::make_shared<Network::Player>(addr.first, addr.second);

    if (player) {
        const std::uint32_t id = player->GetId();

        Storage::Cache::Player::GetInstance().AddPlayer(socket, player);
        Action::Dispatcher::SendMessage(ActionType::DFY, id, Action::List::DFY::State::RequestCredentials);

        return socket;
    }
    return std::nullopt;
}

void Network::Transceiver::HandleDisconnection(const Wrapper::Socket::SocketType socket)
{
    std::optional<std::uint32_t> id = Storage::Cache::Player::GetInstance().GetPlayerIdBySocket(socket);

    if (id.has_value()) {
        const std::shared_ptr<Engine::Game>& game = Storage::Cache::Game::GetInstance().GetGameByPlayerId(id.value());

        if (game) {
            game->RemovePlayerId(id.value());
            Storage::Cache::Game::GetInstance().RemovePlayerFromGame(id.value());
        }

        Storage::Cache::Player::GetInstance().RemovePlayer(socket);
        Wrapper::Socket::Close(socket);
    } else {
        Misc::Logger::Log(std::format("Disconnection of unknown client on socket {}", socket), Misc::Logger::LogLevel::Caution);
    }

    {
        std::unique_lock<std::shared_mutex> lock(_mutex);
        _clients.erase(std::remove_if(_clients.begin(), _clients.end(), [socket](const Wrapper::Socket::PollType& client) {
            return client.fd == socket;
        }), _clients.end());
    }
}

Wrapper::Socket::SocketType Network::Transceiver::CreateSocket(const Wrapper::Socket::Protocol& protocol)
{
    Wrapper::Socket::SocketType socket = Wrapper::Socket::Create(protocol);

    if (!Wrapper::Socket::IsValid(socket)) {
        throw Exception::GenericError(std::format("Failed to create socket: {}", Wrapper::Socket::GetLastError()));
    }
    if (!Wrapper::Socket::SetReuse(socket)) {
        throw Exception::GenericError(std::format("Failed to set socket options: {}", Wrapper::Socket::GetLastError()));
    }
    if (!Wrapper::Socket::Bind(socket, Misc::Env::GetInstance().GetConfiguration<Misc::Env::Server>().port)) {
        throw Exception::GenericError(std::format("Failed to bind socket: {}", Wrapper::Socket::GetLastError()));
    }
    if (protocol != Wrapper::Socket::Protocol::UDP) {
        if (!Wrapper::Socket::Listen(socket)) {
            throw Exception::GenericError(std::format("Failed to listen on socket: {}", Wrapper::Socket::GetLastError()));
        }
    }
    return socket;
}

Wrapper::Socket::SocketType Network::Transceiver::GetSecureSocket() const
{
    return _sockets.tcp;
}

Wrapper::Socket::SocketType Network::Transceiver::GetSocket() const
{
    return _sockets.udp;
}

const std::vector<Wrapper::Socket::PollType>& Network::Transceiver::GetClients() const
{
    return _clients;
}
