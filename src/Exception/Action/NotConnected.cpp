/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NotConnected.cpp
*/

#include "Exception/Action/NotConnected.hpp"

#include <format>

Exception::Action::NotConnectedError::NotConnectedError(const std::string& type, const Wrapper::Socket::SocketType socket, const std::source_location &where) : Exception::GenericError(FormatError(type, socket), where) {}

const std::string Exception::Action::NotConnectedError::FormatError(const std::string& type, const Wrapper::Socket::SocketType socket) const
{
    return std::format("The player on socket {} is not connected and cannot perform action: {}", socket, type);
}
