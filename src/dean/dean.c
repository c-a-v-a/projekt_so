#include "dean.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>

#include "../cli_parser.h"
#include "../ipc_wrapper.h"
#include "../linked_list.h"
#include "../logger.h"

int main(int argc, char** argv) {
  struct DeanArguments args = initial_dean();
  int dean_shmid = get_dean_shmid();
  int semaphore_id = get_semid();
  int* k = (int*)shmat(dean_shmid, NULL, 0);
  int board_a_msgqid = get_board_a_msgqid();
  int board_b_msgqid = get_board_b_msgqid();
  struct Message message;
  struct Node* head = NULL;

  setpgid(0, 0);
  srand(getpid());

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

  if (!log_dean_spawned(args)) {
    perror("Dean error. Failed to log program state");
  }

  *k = rand() % args.k;
  sem_post(semaphore_id, DEAN_SEMAPHORE, 0);
  logger(DEAN_PREFIX, "Sent message %d\n", *k);

  message.slot1 = -11;

  while(1) {
    msgrcv(board_a_msgqid, &message, MESSAGE_SIZE, MESSAGE_SEND_TO_DEAN, 0);

    if (message.slot1 < -10) continue;
    if (message.slot1 < 0) break;

    linked_list_add(&head, (int)message.slot1, (int)message.slot2, message.slot3, -1);
  }

  while (1) {
    msgrcv(board_b_msgqid, &message, MESSAGE_SIZE, MESSAGE_SEND_TO_DEAN, 0);

    if (message.slot1 < 0) break;

    struct Node* curr = head;
    while (curr != NULL) {
      if (curr->k == message.slot1 && curr->n == message.slot2) {
        curr->grade_b = message.slot3;
      }

      curr = curr->next;
    }
  }

  logger(DEAN_PREFIX, "GRADES\n");
  
  struct Node* curr = head;
  while (curr != NULL) {
    if (curr->grade_b < 0) {
      logger(DEAN_PREFIX, "Student[%d,%d] A: %.1f B: NA Final grade: NA\n",
        curr->k, curr->n, curr->grade_a);
    } else {
      logger(DEAN_PREFIX, "Student[%d,%d] A: %.1f B: %.1f Final grade: %.1f\n",
        curr->k, curr->n, curr->grade_a, curr->grade_b, get_average(curr->grade_a, curr->grade_b));
    }

    curr = curr->next;
  }

  linked_list_free(head);

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
  if (signal == SIGUSR1) {
    int pgid_shmid = get_pgid_shmid();
    pid_t* pgid = (pid_t*)shmat(pgid_shmid, NULL, 0);
    logger(DEAN_PREFIX, "Dean sent evacuation signal\n");
    kill(-(*pgid), SIGUSR1);
  }
}

float get_average(float x, float y) {
  if (x < 3 || y < 3) return 2.0;

  float avg = (x + y) / 2;

  for (int i = 0; i < GRADES_LEN; i++) {
    if (GRADES[i] >= avg) return GRADES[i];
  }

  return -1;
}
