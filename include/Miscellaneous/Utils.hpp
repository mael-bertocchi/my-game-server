/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Utils.hpp
*/

#pragma once

#include "Exception/Generic.hpp"

#include <type_traits>
#include <cstring>
#include <cstdint>
#include <string>
#include <vector>

/**
 * @namespace Misc
 * @brief Contains miscellaneous utility classes and functions.
 */
namespace Misc
{
    /**
     * @class Utils
     * @brief This class provides different utility functions.
     */
    class Utils
    {
        public:
            /**
             * @brief Splits a string into a vector of substrings based on a delimiter
             *
             * @param str The input string.
             * @param delimiter The character used to split the string (default is space).
             * @return A vector containing the split substrings.
             */
            static const std::vector<std::string> SplitStr(const std::string& str, char delimiter = ' ');

            /**
             * @brief Convert a string to lowercase
             *
             * @param str The input string
             * @return The lowercase version of the string
             */
            static const std::string ToLower(const std::string& str);

            /**
             * @brief Get the value of an option from a list of arguments
             *
             * @param begin The beginning of the list of arguments
             * @param end The end of the list of arguments
             * @param name The name of the option to search for
             * @return The value of the option, or an empty string if not found
             */
            static const std::string GetTextOption(char **begin, char **end, const std::string& name);

            /**
             * @brief Check if an option exists in the command line arguments.
             *
             * @param begin Pointer to the beginning of argv
             * @param end Pointer to the end of argv
             * @param option The option to search for
             * @return True if the option exists, false otherwise
             */
            static bool HasOption(char **begin, char **end, const std::string& option);

            /**
             * @brief Convert bytes to hex string representation
             *
             * @param data The bytes to convert
             * @param filled Whether to fill single digit hex values with leading zero (default is true)
             * @param spaced Whether to add spaces between hex pairs (default is true)
             * @return Hex string representation of the data
             */
            static std::string BytesToHex(const std::vector<std::uint8_t>& data, bool spaced = true);

            /**
             * @brief Generic serialize function for trivially copyable types
             *
             * @tparam T The type to serialize (must be trivially copyable)
             * @param value The value to serialize
             * @return A vector containing the serialized bytes
             */
            template<typename T>
            static std::vector<std::uint8_t> Serialize(const T& value)
            {
                static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable for serialization");
                std::vector<std::uint8_t> message(sizeof(T));

                std::memcpy(message.data(), &value, sizeof(T));
                return message;
            }

            /**
             * @brief Serialize multiple values into a single byte vector
             *
             * @tparam A Variadic template for multiple types (requires at least 2 arguments)
             * @param args The values to serialize
             * @return A vector containing all serialized bytes concatenated
             */
            template<typename... A>
            static std::vector<std::uint8_t> Serialize(const A&... args)
            {
                static_assert(sizeof...(A) >= 2, "Use single-argument serialize for one value");

                constexpr std::size_t size = (sizeof(A) + ...);
                std::vector<std::uint8_t> result = {};
                result.reserve(size);

                (AppendToVector(result, Serialize(args)), ...);
                return result;
            }

            /**
             * @brief Generic deserialize function for trivially copyable types
             *
             * @tparam T The type to deserialize (must be trivially copyable)
             * @param data The byte vector containing the serialized data
             * @param offset The offset in the byte vector to start deserialization (default is 0)
             * @return The deserialized value of type T
             */
            template<typename T>
            static T Deserialize(const std::vector<std::uint8_t>& data, std::size_t offset = 0)
            {
                static_assert(std::is_trivially_copyable_v<T>, "Type must be trivially copyable for deserialization");

                if (offset + sizeof(T) > data.size()) {
                    throw Exception::GenericError("Not enough data to deserialize the requested type");
                }

                T value = {};
                std::memcpy(&value, data.data() + offset, sizeof(T));
                return value;
            }

            /**
             * @brief Get the underlying integer index of an enum value
             *
             * @param value The enum value
             * @return The underlying integer index as 1-byte unsigned integer
             */
            template <typename E>
            static std::uint8_t GetEnumIndex(E value)
            {
                static_assert(std::is_enum_v<E>, "Template parameter must be an enum type");
                return static_cast<std::uint8_t>(value);
            }

            /**
             * @brief Generate the next unique identifier for a given key
             *
             * @param key The key for which to generate the next ID
             * @return The next unique identifier
             */
            static std::uint32_t GetNextId(const std::string& key);

        private:
            /**
             * @brief Helper function to append one vector to another
             *
             * @param target The target vector to append to
             * @param source The source vector to append
             */
            static void AppendToVector(std::vector<std::uint8_t>& target, const std::vector<std::uint8_t>& source);
    };
}
