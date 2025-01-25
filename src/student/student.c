#include "student.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>

#include "../cli_parser.h"
#include "../ipc_wrapper.h"
#include "../logger.h"

volatile sig_atomic_t CLEANUP = 0;

int main(int argc, char** argv) {
  struct StudentArguments args = initial_student();
  int semaphore_id = get_semid();
  int pgid_shmid = get_pgid_shmid();
  int dean_shmid = get_dean_shmid();
  pid_t* pgid = (pid_t*)shmat(pgid_shmid, NULL, 0);
  int* k;
  int msgqid;

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

  if (!log_student_spawned(args)) {
    perror("Student error. Failed to log program state");
  }

  sem_wait(semaphore_id, PGID_SEMAPHORE, 0);
  if (*pgid == 0) {
    *pgid = getpid();
  }
  setpgid(0, *pgid);
  shmdt(pgid);
  sem_post(semaphore_id, PGID_SEMAPHORE, 0);

  sem_wait(semaphore_id, DEAN_SEMAPHORE, 0);
  k = (int*)shmat(dean_shmid, NULL, 0);
  sem_post(semaphore_id, DEAN_SEMAPHORE, 0);

  if (*k != args.k) {
    shmdt(k);
    logger(STUDENT_PREFIX, "Student (%d,%d) exits. Wrong faculty\n", args.k, args.n);

    return EXIT_SUCCESS;
  }

  // BOARD A
  sem_wait(semaphore_id, BOARD_ROOM_A_SEMAPHORE, 0);

  logger(BOARD_ROOM_PREFIX, "Student (%d,%d) entered board A room\n", args.k, args.n);

  sem_wait(semaphore_id, BOARD_A_SEMAPHORE, 0);

  msgqid = get_board_a_msgqid();
  struct Message message;
  message.mtype = MESSAGE_ASK;

  msgsnd(msgqid, &message, MESSAGE_SIZE, 0);
  msgrcv(msgqid, &message, MESSAGE_SIZE, MESSAGE_QUESTIONS, 0);

  message.mtype = MESSAGE_ANSWERS;

  msgsnd(msgqid, &message, MESSAGE_SIZE, 0);
  msgrcv(msgqid, &message, MESSAGE_SIZE, MESSAGE_GRADE, 0);

  logger(BOARD_ROOM_PREFIX, "Student (%d,%d) got grade\n", args.k, args.n);

  sem_post(semaphore_id, BOARD_A_SEMAPHORE, 0);

  sleep(1);

  logger(BOARD_ROOM_PREFIX, "Student (%d,%d) leaved board A room\n", args.k, args.n);

  sem_post(semaphore_id, BOARD_ROOM_A_SEMAPHORE, 0);

  // BOARD B
  /*
  sem_wait(semaphore_id, BOARD_ROOM_B_SEMAPHORE, 0);
  logger(BOARD_ROOM_PREFIX, "Student (%d,%d) entered board B room\n", args.k, args.n);
  sleep(rand() % 10);
  logger(BOARD_ROOM_PREFIX, "Student (%d,%d) leaved board B room\n", args.k, args.n);
  sem_post(semaphore_id, BOARD_ROOM_B_SEMAPHORE, 0);
  */

  // go to board if already has one grade go to second board
  // get questions
  // wait
  // send answers
  // get grades
  // go to board again
  // exit

  srand(getpid());
  sleep(rand() % 10);

  logger(STUDENT_PREFIX, "Student (%d,%d) finished exam\n", args.k, args.n);

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
