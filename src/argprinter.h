// Module that provides quick functionality to print params generated from
// `argparser` module.

#ifndef ARGPRINTER_H
#define ARGPRINTER_H

#include "cli_parser.h"

// No error handling since it is way too rare for printf to fail and it's not
// critical for the program.
void print_dean_args(struct DeanArguments* args);
void print_student_args(struct StudentArguments* args);
void print_board_args(struct BoardArguments* args);

#endif
