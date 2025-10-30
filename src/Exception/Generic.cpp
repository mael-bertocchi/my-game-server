/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Generic.cpp
*/

#include "Exception/Generic.hpp"

Exception::GenericError::GenericError(const std::string& what, const std::source_location& where) : _what(what), _where(where) {}

const char *Exception::GenericError::what() const noexcept
{
    return _what.c_str();
}

const std::source_location& Exception::GenericError::where() const noexcept
{
    return _where;
}

std::ostream& Exception::operator<<(std::ostream& os, const Exception::GenericError& ex)
{
    os << ex.where().file_name() << ":" << ex.where().line() << ":" << ex.where().column() << ": " << ex.what();
    return os;
}
