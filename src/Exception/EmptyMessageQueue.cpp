/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EmptyMessageQueue.cpp
*/

#include "Exception/EmptyMessageQueue.hpp"

Exception::EmptyMessageQueueError::EmptyMessageQueueError(const std::source_location &where) : Exception::GenericError("Empty message queue", where) {}
