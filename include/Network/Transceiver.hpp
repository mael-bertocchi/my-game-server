/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Transceiver.hpp
*/

#pragma once

#include "Network/Protocol/TCP.hpp"
#include "Network/Protocol/UDP.hpp"
#include "Wrapper/Socket.hpp"

#include <shared_mutex>
#include <memory>
#include <atomic>

/**
 * @namespace Network
 * @brief Contains classes and functions related to network operations.
 */
namespace Network
{
    /**
     * @class Transceiver
     * @brief A class responsible for transmitting and receiving data over the network.
     */
    class Transceiver
    {
        public:
            /**
             * @brief Create the transceiver and initialize the socket.
             */
            explicit Transceiver();

            /**
             * @brief Destroy the transceiver and clean up resources.
             */
            ~Transceiver();

            /**
             * @brief Handle TCP receive operations in dedicated thread.
             *
             * @param running Reference to atomic boolean controlling the running state.
             */
            void SecureReceive(const std::atomic<bool>& running);

            /**
             * @brief Handle TCP send operations in dedicated thread.
             *
             * @param running Reference to atomic boolean controlling the running state.
             */
            void SecureSend(const std::atomic<bool>& running);

            /**
             * @brief Handle UDP receive operations in dedicated thread.
             *
             * @param running Reference to atomic boolean controlling the running state.
             */
            void Receive(const std::atomic<bool>& running);

            /**
             * @brief Handle UDP send operations in dedicated thread.
             *
             * @param running Reference to atomic boolean controlling the running state.
             */
            void Send(const std::atomic<bool>& running);

            /**
             * @brief Get the TCP socket for external access.
             */
            Wrapper::Socket::SocketType GetSecureSocket() const;

            /**
             * @brief Get the UDP socket for external access.
             */
            Wrapper::Socket::SocketType GetSocket() const;

            /**
             * @brief Get reference to TCP clients list for thread safety.
             */
            const std::vector<Wrapper::Socket::PollType>& GetClients() const;

        private:
            /**
             * @struct Managers
             * @brief Structure holding protocol managers.
             */
            struct Managers {
                std::unique_ptr<Protocol::UDP> udp; /*!< The user datagram protocol manager */
                std::unique_ptr<Protocol::TCP> tcp; /*!< The transmission control protocol manager */
            };

            /**
             * @struct Sockets
             * @brief Structure holding both TCP and UDP socket descriptors.
             */
            struct Sockets {
                Wrapper::Socket::SocketType udp; /*!< The user datagram protocol socket */
                Wrapper::Socket::SocketType tcp; /*!< The transmission control protocol socket */
            };

            /**
             * @brief Accept a new incoming connection.
             *
             * @return The accepted client socket if a connection was made, std::nullopt otherwise.
             */
            std::optional<Wrapper::Socket::SocketType> HandleConnection();

            /**
             * @brief Handle disconnection of a client.
             *
             * @param socket The socket of the client to disconnect.
             */
            void HandleDisconnection(const Wrapper::Socket::SocketType socket);

            /**
             * @brief Create a socket with the specified protocol.
             *
             * @param protocol The protocol to use.
             * @return The created socket descriptor.
             */
            Wrapper::Socket::SocketType CreateSocket(const Wrapper::Socket::Protocol& protocol);

            std::vector<Wrapper::Socket::PollType> _clients; /*!< List of connected client sockets */
            mutable std::shared_mutex _mutex; /*!< Mutex for thread-safe access to clients list */
            Managers _managers; /*!< The protocol managers */
            Sockets _sockets; /*!< The protocol sockets */
    };
}
