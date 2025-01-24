#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <sys/types.h>

static const int BOARD_MAX_STUDENTS = 3;
static const float GRADES[] = {2.0, 3.0, 3.5, 4.0, 4.5, 5.0};
static const ssize_t GRADES_LEN = 6;
static const int GRADES_PROBABILITY[] = {5, 15, 30, 30, 15, 5};

int main(int argc, char** argv);

void* board_member(void* arg);

bool attach_handler();
void signal_handler(int signal);

#endif
