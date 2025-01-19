// Module that handles the program entry point. It is responsible for spawning
// new processes and handling user's configuration.

#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <sys/types.h>

int main(int argc, char** argv);

// Collection of functions that are responsible for creating new processes
// and executing given programs (dean, board and student). Arguments that
// are passed to these functions directly relate to `MainArgs` structure
// fields
bool dean_runner(int k);
bool board_runner(int* ns, ssize_t ns_len);
bool students_runner(int k, int* ns, int t);

void signal_handler(int signal);
bool handle_signal();

#endif
