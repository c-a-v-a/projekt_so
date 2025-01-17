// Module that provides quick functionality to print params generated from
// `argparser` module.

#ifndef ARGS_PRINTER_H
#define ARGS_PRINTER_H

#include "argparser.h"

void print_main_args(struct MainArgs* args);
void print_dean_args(struct DeanArgs* args);
void print_student_args(struct StudentArgs* args);
void print_board_args(struct BoardArgs* args);

#endif
