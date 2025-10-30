/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NotRegistered.cpp
*/

#include "Exception/Socket/NotRegistered.hpp"

#include <format>

Exception::Socket::NotRegisteredError::NotRegisteredError(const Wrapper::Socket::SocketType socket, const std::source_location &where) : Exception::GenericError(FormatError(socket), where) {}

const std::string Exception::Socket::NotRegisteredError::FormatError(const Wrapper::Socket::SocketType socket) const
{
    return std::format("Socket {} is not registered", socket);
}
