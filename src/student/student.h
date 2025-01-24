#ifndef STUDENT_H
#define STUDENT_H

#include <stdbool.h>

static const int RETAKER_PROBABILITY = 5;

int main(int argc, char** argv);

bool attach_handler();
void signal_handler(int signal);

#endif
