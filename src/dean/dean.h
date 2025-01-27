/**
 * @file dean.h
 * @author Filip Cebula
 * @date 27 Jan 2025
 * @brief Module that contains dean process.
 *
 * The process's role is to select a random faculty that should take the exam,
 * recieve grades from two board processes, and save the grades to a file.
 */

#ifndef DEAN_H
#define DEAN_H

#include <stdbool.h>
#include <sys/types.h>

int main(int argc, char** argv);

/**
 * @brief Attaches the `signal_handler` function to the SIGUSR1.
 * @see signal_handler
 * @return Whether the action failed or succeeded.
 */
bool attach_handler();

/**
 * @brief Sends SIGUSR1 signal to every student process.
 */
void signal_handler(int signal);

#endif
