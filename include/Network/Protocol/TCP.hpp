/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TCP.hpp
*/

#pragma once

#include "Wrapper/Socket.hpp"
#include "Types.hpp"

#include <vector>
#include <cstdint>

/**
 * @namespace Network-Protocol
 * @brief Contains classes and functions related to network protocols.
 */
namespace Network::Protocol
{
    /**
     * @class TCP
     * @brief Manage transmission control protocol communications
     */
    class TCP
    {
        public:
            /**
             * @brief Look for messages from the client and process them
             *
             * @param socket The client socket to receive data from
             * @return Whether to disconnect the client or not
             */
            bool ReceiveMessage(const Wrapper::Socket::SocketType socket);

            /**
             * @brief Look for messages and send them to the client by session identifier
             *
             * @param socket The client socket to send data to
             */
            void SendMessage(const Wrapper::Socket::SocketType socket);

        private:
            /**
             * @struct Header
             * @brief Structure representing the header of a message.
             */
            struct Header {
                ActionType type; /*!< The type of the message */
                std::uint32_t length; /*!< The length of the message body */
            };

            /**
             * @brief Create a message header given its type and length.
             *
             * @param header The header of the message.
             * @return A vector of bytes representing the message header.
             */
            std::vector<std::uint8_t> SerializeHeader(const Header& header);

            /**
             * @brief Parse the header of a message to extract its type and length.
             *
             * @param content The header of the message as a vector of bytes.
             * @return A Header structure containing the message type and its length.
             */
            Header ParseHeader(const std::vector<std::uint8_t>& content);
    };
}
