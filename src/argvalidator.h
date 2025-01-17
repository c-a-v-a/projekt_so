// Module that takes care of validating arguments parsed in `argparser` module.

#ifndef ARGVALIDATOR_H
#define ARGVALIDATOR_H

#include <stdbool.h>

#include "argparser.h"

// Besides validating if the given arguments are correct, this function also
// fill up with defaults whatever wasn't passed in. It can be only done for
// `main`, since the values in the subprograms are required to match, so they
// need to be passed in via `main` program.
bool validate_main_args(struct MainArgs* args);
bool validate_dean_args(struct DeanArgs* args);
bool validate_student_args(struct StudentArgs* args);
bool validate_board_args(struct BoardArgs* args);

#endif
