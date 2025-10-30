/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Maths.hpp
*/

#pragma once

/**
 * @namespace Misc
 * @brief Contains miscellaneous utility classes and functions.
 */
namespace Misc
{
    /**
     * @class Maths
     * @brief This class provides different mathematical structures and functions.
     */
    class Maths
    {
        public:
            /**
             * @struct Vector2
             * @brief Structure representing a 2D vector.
             *
             * @tparam T The type of the vector components.
             */
            template<typename T>
            struct Vector2 {
                T x; /*!< The x component of the vector */
                T y; /*!< The y component of the vector */
            };
    };
}
