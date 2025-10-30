/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Exception.hpp
*/

#pragma once

#include <source_location>
#include <exception>
#include <ostream>
#include <string>

/**
 * @namespace Exception
 * @brief Contains exception-related classes and functions.
 */
namespace Exception
{
    /**
     * @class Generic
     * @brief The base class for all custom exceptions.
     */
    class GenericError : public std::exception
    {
        public:
            /**
             * @brief Constructs an Exception with a message and source location
             *
             * @param what The error message describing the exception.
             * @param where The location where the exception was thrown (default: current location).
             */
            explicit GenericError(const std::string& what, const std::source_location& where = std::source_location::current());

            /**
             * @brief Retrieves the exception message
             *
             * Overrides std::exception::what() to return the stored error message.
             *
             * @return A C-style string containing the error message.
             */
            const char *what() const noexcept override;

            /**
             * @brief Retrieves the source location where the exception was thrown
             *
             * This function provides debugging information, including the file name,
             * line number, and function where the exception was triggered.
             *
             * @return A reference to the source location of the exception.
             */
            const std::source_location &where() const noexcept;

        private:
            std::string _what; /**< The error message describing the exception. */
            std::source_location _where; /**< The location where the exception was thrown. */
    };

    /**
     * @brief Outputs the exception details to an output stream
     *
     * This function formats the exception message and source location into a human-readable
     * string and writes it to the provided output stream.
     *
     * @param os The output stream to write the exception details to.
     * @param exception The exception object to output.
     * @return A reference to the output stream.
     */
    std::ostream &operator<<(std::ostream& os, const GenericError& exception);
}
