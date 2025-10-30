/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** UDP.hpp
*/

#pragma once

#include "Wrapper/Socket.hpp"

/**
 * @namespace Network-Protocol
 * @brief Contains classes and functions related to network protocols.
 */
namespace Network::Protocol
{
    /**
     * @class UDP
     * @brief User datagram protocol manager
     */
    class UDP
    {
        public:
            /**
             * @brief Create the user datagram protocol manager
             *
             * @param socket The socket to use for communication
             */
            UDP(const Wrapper::Socket::SocketType socket);

            /**
             * @brief Receive a message from a client (simple, direct processing)
             */
            void ReceiveMessage();

            /**
             * @brief Send a message to a client (simple, direct sending)
             *
             * @param socket The client socket to send data to
             */
            void SendMessage(const Wrapper::Socket::SocketType socket);

        private:
            Wrapper::Socket::SocketType _socket; /*!< The socket used for communication */
    };
}
