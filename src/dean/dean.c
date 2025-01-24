#include "dean.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>

#include "../cli_parser.h"
#include "../logger.h"

volatile sig_atomic_t CLEANUP = 0;

int main(int argc, char** argv) {
  struct DeanArguments args = initial_dean();

  if (!parse_dean(argc, argv, &args)) {
    perror("Dean error. Failed to parse arguments");
    return EXIT_FAILURE;
  }

  if (!validate_dean(args)) {
    errno = EINVAL;
    perror("Dean error. Failed to validate arguments");
    return EXIT_FAILURE;
  }

  if (!attach_handler()) {
    perror("Dean error. Failed to attach signal handler");
    return EXIT_FAILURE;
  }

  log_dean_spawned(args);

  return EXIT_SUCCESS;
}

bool attach_handler() {
  struct sigaction sa;

  sa.sa_handler = signal_handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);

  if (sigaction(SIGUSR1, &sa, NULL) == -1) return false;

  return true;
}

void signal_handler(int signal) {
  if (signal == SIGUSR1 && CLEANUP == 0) {
    printf("DEAN: SIGUSR1\n");
    CLEANUP = 1;
  }
}
