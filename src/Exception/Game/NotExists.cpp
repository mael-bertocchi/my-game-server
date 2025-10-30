/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NotExists.cpp
*/

#include "Exception/Game/NotExists.hpp"

#include <format>

Exception::Game::NotExistsError::NotExistsError(const std::uint32_t id, const std::source_location &where) : Exception::GenericError(FormatError(id), where) {}

const std::string Exception::Game::NotExistsError::FormatError(const std::uint32_t id) const
{
    return std::format("Game {} does not exist", id);
}
