/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AlreadyRegistered.cpp
*/

#include "Exception/Socket/AlreadyRegistered.hpp"

#include <format>

Exception::Socket::AlreadyRegisteredError::AlreadyRegisteredError(const Wrapper::Socket::SocketType socket, const std::source_location &where) : Exception::GenericError(FormatError(socket), where) {}

const std::string Exception::Socket::AlreadyRegisteredError::FormatError(const Wrapper::Socket::SocketType socket) const
{
    return std::format("Socket {} is already registered", socket);
}
