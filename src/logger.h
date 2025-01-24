#ifndef LOGGER_H
#define LOGGER_H

#include <stdbool.h>
#include <stdio.h>

#include "cli_parser.h"

static const char LOG_FILE_NAME[] = "./log.txt";

bool open_log_file();
bool close_log_file();

bool logger(char* format, ...);

bool log_dean_spawned(struct DeanArguments arguments);
bool log_board_spawned(struct BoardArguments arguments);
bool log_student_spawned(struct StudentArguments arguments);

#endif
