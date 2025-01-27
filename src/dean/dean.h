#ifndef DEAN_H
#define DEAN_H

#include <stdbool.h>
#include <sys/types.h>

static const float GRADES[] = {2.0, 3.0, 3.5, 4.0, 4.5, 5.0};
static const ssize_t GRADES_LEN = 6;

int main(int argc, char** argv);

bool attach_handler();
void signal_handler(int signal);

float get_average(float x, float y);

#endif
