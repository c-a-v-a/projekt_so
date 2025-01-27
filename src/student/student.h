/**
 * @file student.h
 * @authore Filip Cebula
 * @date 27 Jan 2025
 * @breif Module that containse student process.
 */
#ifndef STUDENT_H
#define STUDENT_H

#include <stdbool.h>

#include "../cli_parser.h"

/**
 * @breif Chance that a student is retaking exam (1=1%).
 */
static const int RETAKER_PROBABILITY = 5;

int main(int argc, char** argv);

/**
 * @brief Attaches the `signal_handler` function to the SIGUSR1.
 * @see signal_handler
 * @return Whether the action failed or succeeded.
 */
bool attach_handler();

/**
 * @brief Ends the process
 */
void signal_handler(int signal);

/**
 * @brief Comunicates with board process via message queues.
 *
 * @param semid ID of semaphore file.
 * @param sem_room Semaphore for board room.
 * @param sem Semaphore for board comittee.
 * @param msgqid ID of a message queue of the board we want to talk with.
 * @param prefix Decides how logger should color stdout (see `logger` module).
 * @param retaker If student is a retaker.
 * @param args Command line arguments passed to student.
 * @param grade Current student grade. Only used if student is a retaker.
 * @param board_name Name of the board we want to communicate with.
 *
 * @return -1 if error ocurred, otherwise returns a grade obtained by the
 * student.
 */
float board_action(int semid, int sem_room, int sem, int msgqid,
    const char* prefix, bool retaker, struct StudentArguments args,
    float grade, char board_name);

#endif
