/**
 * @file board.h
 * @author Filip Cebula
 * @date 27 Jan 2025
 * @brief Module that contains the board process.
 *
 * The process's role is to give questions to students and grade them. The
 * grades are then stored in linked list and sent to the dean process. The
 * board process consists of three members, that are represented as a threads.
 */

#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <sys/types.h>

int main(int argc, char** argv);

/**
 * @brief Generates a random set of questions to send to the student.
 * @param arg The actual parameter is of type `struct Message` from the
 * `ipc_wrapper` module.
 * @return Function changes `slot1`, `slot2` and `slot3` fields of the argument,
 * and sets them to a random number from 1 to 10.
 */
void* prepare_questions(void* arg);

/**
 * @brief Generates a random grades for student.
 *
 * Three randomly generated grades (each with a certain probability) are
 * assigned for every question, and then the average is taken from all of those
 * grades to determine the final grade. If even one grade is 2.0, the student
 * automatically fails, and the total grade is set to 2.0.
 * @param arg The actual parameter is of type `struct Message` from the
 * `ipc_wrapper` module.
 * @return Function changes `slot1`, `slot2`, `slot3` and `total` fields of the
 * argument, and sets them to random grade (from the `grades` module) or an
 * average (from the `grades` module as well).
 *
 * @see GRADES
 * @see get_average
 */
void* grade(void* arg);

#endif
