#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../argparser.h"
#include "../argprinter.h"
#include "../argvalidator.h"

void signal_handler(int signal) {
  if (signal == SIGUSR1) {
    printf("DEAN: SIGUSR1\n");
    // TODO: Clean up
    // EINTR errno?
    exit(1);
  }
}

int main(int argc, char** argv) {
  if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
    perror("Unable to register SIGUSR1 handler");
    exit(1);
  }

  struct DeanArgs args = dean_args_init();
  argparse_dean(argc, argv, &args);

  if (!validate_dean_args(&args)) {
    errno = EINVAL;
    perror("Error while parsing Dean arguments");
    return EXIT_FAILURE;
  }

  print_dean_args(&args);

  while (1) {
    sleep(10);
  }
}
