#ifndef DEAN_H
#define DEAN_H

#include <signal.h>
#include <stdbool.h>

int main(int argc, char** argv);

bool attach_handler();
void signal_handler(int signal);

#endif
