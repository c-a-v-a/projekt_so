#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>

#include "../argparser.h"
#include "../argprinter.h"
#include "../argvalidator.h"
#include "../my_semaphores.h"
#include "../my_shm.h"

volatile sig_atomic_t CLEANUP = 0;

void signal_handler(int signal) {
  if (signal == SIGUSR1 && CLEANUP == 0) {
    printf("STUDENT: SIGUSR1\n");
    exit(EXIT_SUCCESS);
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

  int semid = get_semid();
  int shmid = get_dean_shmid();
  int* shm_ptr = shmat(shmid, NULL, 0);

  sem_wait(semid, DEAN_SEMAPHORE);
  printf("MESSAGE RECIEVED (%d) %d %d\n", *shm_ptr, args.k, args.n);
  sem_post(semid, DEAN_SEMAPHORE);

  return EXIT_SUCCESS;
}
