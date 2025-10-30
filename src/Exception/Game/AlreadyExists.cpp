/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AlreadyExists.cpp
*/

#include "Exception/Game/AlreadyExists.hpp"

#include <format>

Exception::Game::AlreadyExistsError::AlreadyExistsError(const std::uint32_t id, const std::source_location &where) : Exception::GenericError(FormatError(id), where) {}

const std::string Exception::Game::AlreadyExistsError::FormatError(const std::uint32_t id) const
{
    return std::format("Game {} already exists", id);
}
