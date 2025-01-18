// Module that provides quick functionality to print params generated from
// `argparser` module.

#ifndef ARGPRINTER_H
#define ARGPRINTER_H

#include "argparser.h"

// No error handling since it is way too rare for printf to fail and it's not
// critical for the program.
void print_main_args(struct MainArgs* args);
void print_dean_args(struct DeanArgs* args);
void print_student_args(struct StudentArgs* args);
void print_board_args(struct BoardArgs* args);

#endif
