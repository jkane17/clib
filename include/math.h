
/*
    File Type   : C Header
    Description : Mathematical Operations
*/

#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

/**
 * @brief Compute the absolute value of x.
 * 
 * @param x : Numeric value
 * @return Absolute value
 */
#define math_abs(x) (x < 0 ? -1 * x : x)

/**
 * @brief Compute the minimum value between x and y.
 * 
 * @param x : Numeric value
 * @param y : Numeric value
 * @return Minimum value
 */
#define math_min(x, y) (x < y ? x : y)

#endif
