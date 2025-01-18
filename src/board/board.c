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
    printf("BOARD: SIGUSR1\n");
    CLEANUP = 1;
  }
}

int main(int argc, char** argv) {
  if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
    perror("Unable to register SIGUSR1 handler");
    exit(1);
  }

  struct BoardArgs args = board_args_init();
  argparse_board(argc, argv, &args);

  if (!validate_board_args(&args)) {
    if (args.ns != NULL) {
      free(args.ns);
    }
    errno = EINVAL;
    perror("Error while parsing Board arguments");
    return EXIT_FAILURE;
  }

  print_board_args(&args);

  while (1) {
    sleep(2);

    if (CLEANUP == 1) {
      if (args.ns != NULL) free(args.ns);
      return EXIT_SUCCESS;
    }
  }
}
