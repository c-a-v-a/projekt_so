/**
 * @file logger.h
 * @author Filip Cebula
 * @date 27 Jan 2025
 * @brief Module that provides printf wrapper utility.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdbool.h>
#include <stdio.h>

#include "cli_parser.h"

/**
 * @brief Prefixes to use for logging. They provide a color for the output.
 */
static const char BOARD_PREFIX[] = "\033[31m";
static const char DEAN_PREFIX[] = "\033[32m";
static const char MAIN_PREFIX[] = "\033[33m";
static const char STUDENT_PREFIX[] = "\033[34m";
static const char BOARD_ROOM_A_PREFIX[] = "\033[35m";
static const char BOARD_ROOM_B_PREFIX[] = "\033[36m";
static const char BOARD_ANSWER_PREFIX[] = "\033[36m";

/**
 * @brief Suffix to use for logging. It clears color from stdout.
 */
static const char SUFFIX[] = "\033[0m";

/**
 * @brief Wrapper for printf, that allows to set color via prefix.
 *
 * @param prefix Prefix to use for coloring output.
 * @param format Format just like in printf.
 */
bool logger(const char* prefix, char* format, ...);

/**
 * @brief Log when process was spawned, it prints what process was spawned and
 * it's parameters.
 */
bool log_board_spawned(struct BoardArguments arguments);
bool log_dean_spawned(struct DeanArguments arguments);
bool log_main_spawned(struct MainArguments arguments);
bool log_student_spawned(struct StudentArguments arguments);

#endif
