/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Socket.hpp
*/

#pragma once

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <poll.h>
#endif

#include <functional>
#include <optional>
#include <utility>
#include <cstdint>
#include <string>
#include <vector>
#include <map>

#include "Variables.hpp"

/**
 * @namespace Wrapper
 * @brief Contains wrapper classes for system-level operations.
 */
namespace Wrapper
{
    /**
     * @class Socket
     * @brief A wrapper class for socket operations.
     */
    class Socket
    {
        public:
#ifdef _WIN32
            /**
             * @typedef SocketType
             * @brief Type alias for socket descriptor on Windows systems
             */
            using SocketType = SOCKET;

            /**
             * @typedef TransmitType
             * @brief Type alias for send function return type on Windows systems
             */
            using TransmitType = std::int32_t;

            /**
             * @typedef PollType
             * @brief Type alias for poll structure on Windows systems
             */
            using PollType = WSAPOLLFD;
#else
            /**
             * @typedef SocketType
             * @brief Type alias for socket descriptor on Unix systems
             */
            using SocketType = std::int32_t;

            /**
             * @typedef TransmitType
             * @brief Type alias for send function return type on Unix systems
             */
            using TransmitType = ssize_t;

            /**
             * @typedef PollType
             * @brief Type alias for poll structure on Unix systems
             */
            using PollType = struct pollfd;
#endif

            /**
             * @enum Protocol
             * @brief Enumeration for socket protocols.
             */
            enum class Protocol {
                TCP, /*!< We use transmission control protocol */
                UDP /*!< We use user datagram protocol */
            };

            /**
             * @brief Initialize the socket library (needed for Windows)
             * @return True if initialization succeeded, false otherwise
             */
            static bool Initialize();

            /**
             * @brief Clean up the socket library (needed for Windows)
             */
            static void Cleanup();

            /**
             * @brief Check if a socket is valid
             *
             * @param sock Socket descriptor to check
             * @return True if the socket is valid, false otherwise
             */
            static bool IsValid(const SocketType& sock);

            /**
             * @brief Create a socket with the specified protocol
             *
             * @param protocol The protocol to use (TCP or UDP)
             * @return Socket descriptor or INVALID_SOCKET/(-1) on error
             */
            static SocketType Create(const Protocol& protocol);

            /**
             * @brief Close a socket
             *
             * @param sock Socket descriptor to close
             * @return True if the socket was closed successfully, false otherwise
             */
            static bool Close(const SocketType& sock);

            /**
             * @brief Connect to a remote server
             *
             * @param sock Socket descriptor
             * @param address IP address to connect to
             * @param port Port to connect to
             * @return True if connected successfully, false otherwise
             */
            static bool Connect(const SocketType& sock, const std::string& address, std::uint16_t port);

            /**
             * @brief Bind a socket to a local address and port
             *
             * @param sock Socket descriptor
             * @param port Port to bind to
             * @param address Optional IP address to bind to (empty for any)
             * @return True if bound successfully, false otherwise
             */
            static bool Bind(const SocketType& sock, const std::uint16_t port, const std::string& address = "");

            /**
             * @brief Start listening for incoming connections
             *
             * @param sock Socket descriptor
             * @param backlog Maximum length of the queue of pending connections
             * @return True if listening successfully, false otherwise
             */
            static bool Listen(const SocketType& sock, std::int32_t backlog = 10);

            /**
             * @brief Accept an incoming connection
             *
             * @param sock Socket descriptor
             * @param data Optional reference to a pair that will store client address and port
             * @return Socket descriptor for the new connection or INVALID_SOCKET/(-1) on error
             */
            static SocketType Accept(const SocketType& sock, std::optional<std::reference_wrapper<std::pair<std::string, std::uint16_t>>> data = std::nullopt);

            /**
             * @brief Set the socket option to reuse the address
             *
             * @param sock Socket descriptor
             * @param reuse True to enable address reuse, false to disable
             */
            static bool SetReuse(const SocketType& sock, bool reuse = true);

            /**
             * @brief Send data over transmission control protocol
             *
             * @param sock Socket descriptor
             * @param data The vector of bytes to send
             * @return Number of bytes sent or -1 on error
             */
            static TransmitType Send(const SocketType& sock, const std::vector<std::uint8_t>& data);

            /**
             * @brief Receive data over transmission control protocol
             *
             * @param sock Socket descriptor
             * @param size Maximum number of bytes to receive
             * @return Vector containing the received data or empty vector on error
             */
            static std::vector<std::uint8_t> Receive(const SocketType& sock, std::size_t size);

            /**
             * @brief Poll a set of sockets for events
             *
             * @param fds Vector of PollType structures containing sockets to monitor
             * @param timeout Timeout in milliseconds (-1 for infinite wait)
             * @return Number of sockets with events, 0 on timeout, or -1 on error
             */
            static std::int32_t Poll(std::vector<PollType>& fds, std::int32_t timeout = 0);

            /**
             * @brief Receive data from a datagram socket and get sender endpoint
             *
             * @param sock UDP socket descriptor
             * @param address Reference to string to store sender address
             * @param port Reference to store sender port
             * @return Vector containing the received data or empty vector on error
             */
            static std::vector<std::uint8_t> ReceiveFrom(const SocketType& sock, std::string& address, std::uint16_t& port);

            /**
             * @brief Send data to a specific address/port using a datagram socket
             *
             * @param sock UDP socket to send from
             * @param data The vector of bytes to send
             * @param address Destination IPv4 address as string
             * @param port Destination port
             * @return Number of bytes sent or -1 on error
             */
            static TransmitType SendTo(const SocketType& sock, const std::vector<std::uint8_t>& data, const std::string& address, std::uint16_t port);

            /**
             * @brief Get the last socket error as a string
             * @return Error message
             */
            static std::string GetLastError();
    };
}
