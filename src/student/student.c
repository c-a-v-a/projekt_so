#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../argparser.h"
#include "../argprinter.h"
#include "../argvalidator.h"

volatile sig_atomic_t CLEANUP = 0;

void signal_handler(int signal) {
  if (signal == SIGUSR1 && CLEANUP == 0) {
    printf("STUDENT: SIGUSR1\n");
    CLEANUP = 1;
  }
}

int main(int argc, char** argv) {
  if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
    perror("Unable to register SIGUSR1 handler");
    exit(1);
  }

  struct StudentArgs args = student_args_init();
  argparse_student(argc, argv, &args);

  if (!validate_student_args(&args)) {
    errno = EINVAL;
    perror("Error while parsing Student arguments");
    return EXIT_FAILURE;
  }

  print_student_args(&args);

  while (1) {
    sleep(2);

    // Clean up goes here
    if (CLEANUP == 1) return EXIT_SUCCESS;
  }
}
