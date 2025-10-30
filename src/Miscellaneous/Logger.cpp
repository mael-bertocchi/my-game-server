/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Logger.cpp
*/

#include "Miscellaneous/Logger.hpp"

#include <iostream>
#include <iomanip>
#include <chrono>

Misc::Logger::Logger() : _enabled(false) {}

void Misc::Logger::SetDebugMode(bool enable)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _enabled = enable;
}

bool Misc::Logger::IsDebugEnabled() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _enabled;
}

void Misc::Logger::WriteLogEntry(const std::string& message, const LogLevel level)
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (_enabled) {
        auto currentTimePoint = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(currentTimePoint);

#ifdef _WIN32
        std::tm localTimeStorage = {};

        if (localtime_s(&localTimeStorage, &timeT) != 0) {
            throw Exception::GenericError("Failed to obtain local time");
        }
        std::tm* localTimeInfo = &localTimeStorage;
#else
        std::tm* localTimeInfo = std::localtime(&timeT);
#endif

        switch (level) {
            case LogLevel::Default:
                std::cerr << "\r[" << std::put_time(localTimeInfo, "%Y-%m-%d %H:%M:%S") << "] \033[0;32m>\033[0;0m " << message << std::endl;
                break;
            case LogLevel::Caution:
                std::cout << "\r[" << std::put_time(localTimeInfo, "%Y-%m-%d %H:%M:%S") << "] \033[0;33m>\033[0;0m " << message << std::endl;
                break;
            case LogLevel::Critical:
                std::cout << "\r[" << std::put_time(localTimeInfo, "%Y-%m-%d %H:%M:%S") << "] \033[0;31m>\033[0;0m " << message << std::endl;
                break;
            default:
                break;
        }
    }
}

void Misc::Logger::Log(const std::string& message, const LogLevel level)
{
    GetInstance().WriteLogEntry(message, level);
}
