// Module that handles the program entry point. It is responsible for spawning
// new processes and handling user's configuration.

#ifndef MAIN_H
#define MAIN_H

#include <sys/types.h>

int main(int argc, char** argv);

// Collection of functions that are responsible for creating new processes
// and executing given programs (dean, board and student), since all those
// processes are spawned differently (eg. different amount or different
// arguments that need to be passed to spawned program).
void dean_runner();
void board_runner(char* boards, size_t boards_length);
void students_runner(int k, int* ns);

#endif
