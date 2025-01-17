// Module that defines default configuration, and functions to obtain certain
// configuration values without user input.

#ifndef DEFAULTS_H
#define DEFAULTS_H

#include <sys/types.h>

#if defined(ENV_DEV)

// Smaller numbers for testing purposes
static const int DEFAULT_K = 2;
static const int MIN_N_RANGE = 8;
static const int MAX_N_RANGE = 16;

#else

static const int DEFAULT_K = 5;

// Range for random generation of Ns parameters
static const int MIN_N_RANGE = 80;
static const int MAX_N_RANGE = 160;

#endif

static const char BOARDS[] = {'A', 'B'};
static const size_t BOARDS_LEN = 2;

// How many students can enter the board room at once, and wait for their
// questions.
static const int BOARD_MAX_STUDENTS = 3;

static const float GRADES[] = {2.0, 3.0, 3.5, 4.0, 4.5, 5.0};
static const size_t GRADES_LEN = 6;

// Distribution of probabilities for any given grade that student can get,
// where 1 represents 1%, since grade is determined by a random number from
// 1 to 100.
static const int GRADES_PROBABILITY[] = {5, 15, 30, 30, 15, 5};

// Chance that a given student is retaking the exam and is allowed to show
// his old grade to the `board A` and go directly to `board B`. 1 = 1%.
static const int RETAKER_PROBABILITY = 5;

// Generate an array of length `k`, that holds how many students study in
// each of `k` fields.
int* generate_random_ns(int k, int min, int max);

// Generates random grade with given probability.
// TODO: int generate_random_grade(int* grades, int* probabilites, size_t
// length);

#endif
