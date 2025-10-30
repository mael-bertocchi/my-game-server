/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Password.cpp
*/

#include "Miscellaneous/Password.hpp"
#include "Exception/Generic.hpp"
#include "Miscellaneous/Utils.hpp"
#include "Variables.hpp"

#include <openssl/rand.h>
#include <openssl/evp.h>
#include <vector>

std::string Misc::Password::HashPassword(const std::string& password)
{
    std::vector<uint8_t> salt(CRYPTION_SALT_LENGTH);

    if (!RAND_bytes(salt.data(), salt.size())) {
        throw Exception::GenericError("Failed to generate random salt for password hashing");
    }

    std::vector<uint8_t> hash(CRYPTION_HASH_LENGTH);
    if (!PKCS5_PBKDF2_HMAC(password.c_str(), static_cast<int>(password.size()), salt.data(), static_cast<int>(salt.size()), CRYPTION_ITERATIONS, EVP_sha256(), static_cast<int>(hash.size()), hash.data())) {
        throw Exception::GenericError("Failed to derive PBKDF2 hash");
    }

    std::vector<uint8_t> combined = {};
    combined.reserve(salt.size() + hash.size());
    combined.insert(combined.end(), salt.begin(), salt.end());
    combined.insert(combined.end(), hash.begin(), hash.end());

    return std::to_string(CRYPTION_ITERATIONS) + "$" + Utils::BytesToHex(combined, false);
}

bool Misc::Password::VerifyPassword(const std::string& password, const std::string& stored)
{
    const auto pos = stored.find('$');

    if (pos != std::string::npos) {
        std::int32_t iterations = 0;
        try {
            iterations = std::stoi(stored.substr(0, pos));
        } catch (...) {
            return false;
        }

        const std::string hex = stored.substr(pos + 1);
        if (hex.size() < (CRYPTION_SALT_LENGTH + CRYPTION_HASH_LENGTH) * 2) {
            return false;
        }

        std::vector<std::uint8_t> data = {};
        data.reserve(hex.size() / 2);

        for (std::size_t i = 0; i < hex.size(); i += 2) {
            if (i + 1 >= hex.size()) {
                return false;
            }
            std::uint8_t byte = static_cast<std::uint8_t>(std::stoi(hex.substr(i, 2), nullptr, 16));
            data.push_back(byte);
        }

        if (data.size() != CRYPTION_SALT_LENGTH + CRYPTION_HASH_LENGTH) {
            return false;
        }

        const std::vector<std::uint8_t> salt(data.begin(), data.begin() + CRYPTION_SALT_LENGTH);
        const std::vector<std::uint8_t> storedHash(data.begin() + CRYPTION_SALT_LENGTH, data.end());

        std::vector<std::uint8_t> computed(CRYPTION_HASH_LENGTH);
        if (!PKCS5_PBKDF2_HMAC(password.c_str(), static_cast<int>(password.size()), salt.data(), static_cast<int>(salt.size()), iterations, EVP_sha256(), static_cast<int>(computed.size()), computed.data())) {
            return false;
        }

        return ConstantTimeCompare(computed, storedHash);
    }
    return false;
}

bool Misc::Password::ConstantTimeCompare(const std::vector<std::uint8_t>& a, const std::vector<std::uint8_t>& b)
{
    if (a.size() != b.size()) {
        return false;
    }

    std::uint8_t diff = 0;
    for (std::size_t i = 0; i < a.size(); i++) {
        diff |= a[i] ^ b[i];
    }
    if (diff > 1) {
        return false;
    }
    return true;
}
