
/*
    File        : math.h
    Description : Mathematical utility functions and macros.
*/

#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

/**
 * @brief Compute the maximum value between `x` and `y`.
 * 
 * @param x Numeric value.
 * @param y Numeric value.
 * @return Maximum value of `x` and `y`.
 */
#define math_max(x, y) ((x) > (y) ? (x) : (y))

/**
 * @brief Compute the minimum value between `x` and `y`.
 * 
 * @param x Numeric value.
 * @param y Numeric value.
 * @return Minimum value of `x` and `y`.
 */
#define math_min(x, y) ((x) < (y) ? (x) : (y))

#endif // MATH_H_INCLUDED
