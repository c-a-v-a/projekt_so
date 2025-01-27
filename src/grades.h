#ifndef GRADES_H
#define GRADES_H

#include <sys/types.h>

static const float GRADES[] = {2.0, 3.0, 3.5, 4.0, 4.5, 5.0};
static const ssize_t GRADES_LEN = 6;
static const int GRADES_PROBABILITY[] = {5, 15, 30, 30, 15, 5};

float get_random_grade();

float get_average(float x, float y, float z);

float get_final_grade(float x, float y);

#endif
