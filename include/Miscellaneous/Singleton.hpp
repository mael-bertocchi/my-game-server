/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Singleton.hpp
*/

#pragma once

/**
 * @namespace Misc
 * @brief Contains miscellaneous utility classes and functions.
 */
namespace Misc
{
    /**
     * @class Singleton
     *
     * @brief A template class that implements the Singleton design pattern.
     */
    template <typename T>
    class Singleton
    {
        public:
            /**
             * @brief Deleted copy constructor to prevent copying of the singleton instance.
             *
             * @param other The other instance to copy from.
             */
            explicit Singleton(const Singleton& other) = delete;

            /**
             * @brief Deleted assignment operator to prevent assignment of the singleton instance.
             *
             * @param other The other instance to assign from.
             */
            Singleton& operator=(const Singleton& other) = delete;

            /**
              * @brief Get the singleton instance.
             */
            static T& GetInstance() {
                static T instance;

                return instance;
            }

        protected:
            /**
             * @brief Default constructor for the singleton class.
             */
            Singleton() = default;

            /**
             * @brief Default destructor for the singleton class.
             */
            ~Singleton() = default;
    };
}
