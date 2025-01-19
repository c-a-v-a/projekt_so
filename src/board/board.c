#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../argparser.h"
#include "../argprinter.h"
#include "../argvalidator.h"

volatile sig_atomic_t CLEANUP = 0;

void* board_member(void* arg) {
  int* x = (int*)arg;
  printf("I am a board member %d\n", *x);

  return NULL;
}

void signal_handler(int signal) {
  if (signal == SIGUSR1 && CLEANUP == 0) {
    printf("BOARD: SIGUSR1\n");
    CLEANUP = 1;
  }
}

// TODO: general code cleanup
// TODO: general documentation cleanup
int main(int argc, char** argv) {
  if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
    perror("Unable to register SIGUSR1 handler");
    exit(1);
  }

  pthread_t t1, t2, t3;
  int a = 1;
  int b = 2;
  int c = 3;
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

  pthread_create(&t1, NULL, board_member, (void*)&a);
  pthread_create(&t2, NULL, board_member, (void*)&b);
  pthread_create(&t3, NULL, board_member, (void*)&c);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);

  while (1) {
    sleep(2);

    if (CLEANUP == 1) {
      if (args.ns != NULL) free(args.ns);
      return EXIT_SUCCESS;
    }
  }
}
