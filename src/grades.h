/**
 * @file grades.h
 * @author Filip Cebula
 * @date 27 Jan 2025
 * @brief Module that provides constants and utility functions regarding grades.
 */

#ifndef GRADES_H
#define GRADES_H

#include <sys/types.h>

/**
 * @breif All possible grades that student can obtain.
 */
static const float GRADES[] = {2.0, 3.0, 3.5, 4.0, 4.5, 5.0};
static const ssize_t GRADES_LEN = 6;

/**
 * @breif Probability of getting a given grade (1=1%).
 *
 * Probability is cumulative, meaning that for index 0 it will be 5, but for
 * index 1 it will be 15+5=20. The elements of this array directly corelate
 * with the elements of the `GRADES` array, meaning that 2.0 has 5% probability
 * and so on.
 */
static const int GRADES_PROBABILITY[] = {5, 15, 30, 30, 15, 5};

/**
 * @brief Gets random grade from `GRADES` array with given probability from
 * `GRADES_PROBABILITY` array.
 *
 * @return Grade from `GRADES` array.
 */
float get_random_grade();

/**
 * @brief Calculates a mean of `x`, `y` and `z` and rounds it up to the next
 * grade.
 *
 * @param x Some float number.
 * @param y Some float number.
 * @param z Some float number.
 * @return Grade from `GRADES` array.
 */
float get_average(float x, float y, float z);

/**
 * @brief Same as `get_average` but for two floats.
 *
 * @param x Some float number.
 * @param y Some float number.
 * @return Grade from `GRADES` array.
 */
float get_final_grade(float x, float y);

#endif
