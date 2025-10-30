/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Password.hpp
*/

#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * @namespace Misc
 * @brief Contains miscellaneous utility classes and functions
 */
namespace Misc
{
    /**
     * @class Password
     * @brief A class for handling password hashing and verification
     */
    class Password
    {
        public:
            /**
             * @brief Hash a password using a simple algorithm.
             *
             * @param password The password to hash.
             * @return The hashed password as a string.
             * @throw Exception if hashing fails.
             */
            static std::string HashPassword(const std::string& password);

            /**
             * @brief Verify if a given password matches the hashed password.
             *
             * @param password The plain text password to verify.
             * @param hashed The hashed password to compare against.
             * @return True if the passwords match, false otherwise.
             */
            static bool VerifyPassword(const std::string& password, const std::string& hashed);

        private:
            /**
             * @brief Compare two strings in constant time to prevent timing attacks.
             *
             * @param first The first string to compare.
             * @param second The second string to compare.
             * @return True if the strings are equal, false otherwise.
             */
            static bool ConstantTimeCompare(const std::vector<std::uint8_t>& first, const std::vector<std::uint8_t>& second);
    };
}
