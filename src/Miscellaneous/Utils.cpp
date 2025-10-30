/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Utils.cpp
*/

#include "Miscellaneous/Utils.hpp"
#include "Exception/Generic.hpp"

#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdint>
#include <sstream>
#include <limits>
#include <format>
#include <mutex>

std::vector<std::string> Misc::Utils::Split(const std::string& str, char delimiter)
{
    std::vector<std::string> words {};
    std::stringstream stream(str);
    std::string word {};

    while (std::getline(stream, word, delimiter)) {
        words.push_back(word);
    }
    return words;
}

const std::string Misc::Utils::GetTextOption(char **begin, char **end, const std::string& name)
{
    char **itr = std::find(begin, end, name);

    if (itr != end && ++itr != end) {
        return *itr;
    }
    throw Exception::GenericError(std::format("Missing argument for option {}", name));
}

bool Misc::Utils::HasOption(char **begin, char **end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

std::string Misc::Utils::BytesToHex(const std::vector<std::uint8_t>& data, bool spaced)
{
    std::ostringstream oss = {};

    oss << std::hex << std::nouppercase << std::setfill('0');
    for (std::size_t i = 0; i < data.size(); i++) {
        if (i > 0 && spaced) {
            oss << ' ';
        }
        oss << std::setw(2) << static_cast<std::int32_t>(data[i]);
    }
    return oss.str();
}

std::uint32_t Misc::Utils::GetNextId(const std::string& key)
{
    static std::unordered_map<std::string, std::uint32_t> ids = {};
    static std::mutex mutex = {};

    std::lock_guard<std::mutex> lock(mutex);
    std::uint32_t& actual = ids[key];

    if (actual == 0) {
        actual = 1;
    } else if (actual == std::numeric_limits<std::uint32_t>::max()) {
        actual = 1;
    } else {
        ++actual;
    }

    return actual;
}

void Misc::Utils::AppendToVector(std::vector<std::uint8_t>& target, const std::vector<std::uint8_t>& source)
{
    target.insert(target.end(), source.begin(), source.end());
}
