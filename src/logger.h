#ifndef LOGGER_H
#define LOGGER_H

#include <stdbool.h>
#include <stdio.h>

#include "cli_parser.h"

static const char LOG_FILE_NAME[] = "./log.txt";

static const char BOARD_PREFIX[] = "\033[31m";
static const char DEAN_PREFIX[] = "\033[32m";
static const char MAIN_PREFIX[] = "\033[33m";
static const char STUDENT_PREFIX[] = "\033[34m";
static const char BOARD_ROOM_A_PREFIX[] = "\033[35m";
static const char BOARD_ROOM_B_PREFIX[] = "\033[36m";
static const char BOARD_ANSWER_PREFIX[] = "\033[36m";
static const char SUFFIX[] = "\033[0m";

bool open_log_file();
bool close_log_file();

bool logger(const char* prefix, char* format, ...);

bool log_board_spawned(struct BoardArguments arguments);
bool log_dean_spawned(struct DeanArguments arguments);
bool log_main_spawned(struct MainArguments arguments);
bool log_student_spawned(struct StudentArguments arguments);

#endif
