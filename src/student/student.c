#include "student.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../cli_parser.h"
#include "../ipc_wrapper.h"
#include "../logger.h"

volatile sig_atomic_t CLEANUP = 0;

int main(int argc, char** argv) {
  struct StudentArguments args = initial_student();
  int semaphore_id = get_semid();

  if (!parse_student(argc, argv, &args)) {
    perror("Student error. Failed to parse arguments");
    return EXIT_FAILURE;
  }

  if (!validate_student(args)) {
    errno = EINVAL;
    perror("Student error. Failed to validate arguments");
    return EXIT_FAILURE;
  }

  if (!attach_handler()) {
    perror("Student error. Failed to attach signal handler");
    return EXIT_FAILURE;
  }

  sem_wait(semaphore_id, LOGGER_SEMAPHORE, 0);
  if (!log_student_spawned(args)) {
    perror("Student error. Failed to log program state");
  }
  sem_post(semaphore_id, LOGGER_SEMAPHORE, 0);

  // get dean k
  // go to board room
  // go to board if already has one grade go to second board
  // get questions
  // wait
  // send answers
  // get grades
  // go to board again
  // exit

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
    printf("STUDENT: SIGUSR1\n");
    exit(EXIT_SUCCESS);
  }
}
