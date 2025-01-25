#ifndef STUDENT_H
#define STUDENT_H

#include <stdbool.h>

static const int RETAKER_PROBABILITY = 5;
static const float GRADES[] = {2.0, 3.0, 3.5, 4.0, 4.5, 5.0};

int main(int argc, char** argv);

bool attach_handler();
void signal_handler(int signal);

#endif
