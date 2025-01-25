#include "board.h"

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include "../cli_parser.h"
#include "../ipc_wrapper.h"
#include "../logger.h"

volatile sig_atomic_t CLEANUP = 0;

int main(int argc, char** argv) {
  struct BoardArguments args = initial_board();
  pthread_t t1, t2, t3;
  int a = 1;
  int b = 2;
  int c = 3;
  int msgqid;
  struct Message message;
  int semaphore_id = get_semid();

  setpgid(0, 0);

  if (!parse_board(argc, argv, &args)) {
    perror("Board error. Failed to parse arguments");
    return EXIT_FAILURE;
  }

  if (!validate_board(args)) {
    errno = EINVAL;
    perror("Board error. Failed to validate arguments");
    return EXIT_FAILURE;
  }

  if (!attach_handler()) {
    perror("Board error. Failed to attach signal handler");
    return EXIT_FAILURE;
  }

  if (!log_board_spawned(args)) {
    perror("Board error. Failed to log program state");
  }

  if (args.board_name == 'A') {
    msgqid = get_board_a_msgqid();
  } else {
    msgqid = get_board_b_msgqid();
  }

  while (1) {
    if (sem_wait(semaphore_id, END_SEMAPHORE, IPC_NOWAIT)) {
      sem_post(semaphore_id, END_SEMAPHORE, IPC_NOWAIT);
      break;
    }

    if (msgrcv(msgqid, &message, MESSAGE_SIZE, 0, IPC_NOWAIT) == -1) {
      continue;
    }

    switch(message.mtype) {
      case MESSAGE_ASK:
        message.mtype = MESSAGE_QUESTIONS;
        msgsnd(msgqid, &message, MESSAGE_SIZE, 0);

        break;
      case MESSAGE_ANSWERS:
        message.mtype = MESSAGE_GRADE;
        msgsnd(msgqid, &message, MESSAGE_SIZE, 0);

        break;
      case MESSAGE_RETAKER:
        message.mtype = MESSAGE_GRADE;
        msgsnd(msgqid, &message, MESSAGE_SIZE, 0);

        break;
      default:
        errno = EINVAL;
        perror("Board error. Bad message type");
        break;
    }

    sleep(1);
  }

  pthread_create(&t1, NULL, board_member, (void*)&a);
  pthread_create(&t2, NULL, board_member, (void*)&b);
  pthread_create(&t3, NULL, board_member, (void*)&c);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);

  message.mtype = MESSAGE_SEND_TO_DEAN;
  msgsnd(msgqid, &message, MESSAGE_SIZE, 0);

  return EXIT_SUCCESS;
}

void* board_member(void* arg) {
  int* x = (int*)arg;
  printf("I am a board member %d\n", *x);

  return NULL;
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
    printf("BOARD: SIGUSR1\n");
    CLEANUP = 1;
  }
}
