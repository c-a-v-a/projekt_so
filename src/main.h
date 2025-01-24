// Module that handles the program entry point. It is responsible for spawning
// new processes and handling user's configuration.

#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

#include "cli_parser.h"

int main(int argc, char** argv);

void cleanup(struct MainArguments* arguments);

// Collection of functions that are responsible for creating new processes
// and executing given programs (dean, board and student). Arguments that
// are passed to these functions directly relate to `MainArgs` structure
// fields
bool dean_runner(int k, pid_t* dean);
bool board_runner(int* ns, ssize_t ns_len, pid_t* boards);
bool students_runner(int k, int* ns, int t, pid_t* student_leader);

bool attach_handler();
void signal_handler(int signal);

char* int_to_str(int x);
char* int_arr_to_str(int* xs, ssize_t n);

#endif
