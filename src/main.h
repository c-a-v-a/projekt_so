/**
 * @file main.h
 * @author Filip Cebula
 * @date 27 Jan 2025
 * @brief Module that contains the main program.
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

#include "cli_parser.h"

int main(int argc, char** argv);

/**
 * @brief Cleans up allocated memory for arguments and removes system v
 * structures.
 *
 * @param arguments Parsed command line arguments passed to main.
 */
void cleanup(struct MainArguments* arguments);

/**
 * @brief Creates a dean process.
 *
 * @param k Number of available majors.
 * @param dean Pointer for pid of dean process. It is changed by this function.
 * @return Whether operation failed or succeeded.
 */
bool dean_runner(int k, volatile pid_t* dean);

/**
 * @brief Creates two board processes.
 *
 * @param ns Number of students in given major.
 * @param ns_len Length of ns array.
 * @param boards Array of length 2, that stores the pids of board processes.
 * @return Whether operation failed or succeeded.
 */
bool board_runner(pid_t* boards);

/**
 * @brief Creates a student processes.
 *
 * @param k Number of available majors.
 * @param ns Number of students in given major. Array of length `k`.
 * @param t How long should student wait before answering the questions.
 * @return Whether operation failed or succeeded.
 */
bool students_runner(int k, int* ns, int t);

/**
 * @brief Attaches the `signal_handler` function to the SIGUSR1.
 * @see signal_handler
 * @return Whether the action failed or succeeded.
 */
bool attach_handler();

/**
 * @brief Passes the SIGUSR1 signal to dean process.
 */
void signal_handler(int signal);

/**
 * @brief Integer to string conversion.
 *
 * @param x Number that will be converted.
 * @return Number x in for of a string.
 */
char* int_to_str(int x);

/**
 * @brief Converts array of integers into a string, so that every integer is
 * separated by ','.
 *
 * @param xs Array of integers that we want to convert.
 * @param n Size of the `xs` array.
 * @return Array in form of a string where every int is separated by ','.
 */
char* int_arr_to_str(int* xs, ssize_t n);

/**
 * @breif Waits asynchronously for all students to finish.
 */
void* student_waiter();

#endif
