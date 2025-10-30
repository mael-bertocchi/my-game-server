/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NotImplemented.cpp
*/

#include "Exception/Action/NotImplemented.hpp"

#include <format>

Exception::Action::NotImplementedError::NotImplementedError(const std::string& type, const std::source_location &where) : Exception::GenericError(FormatError(type), where) {}

const std::string Exception::Action::NotImplementedError::FormatError(const std::string& type) const
{
    return std::format("The action is not implemented: {}", type);
}
