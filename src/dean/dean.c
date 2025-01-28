#include "dean.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>

#include "../cli_parser.h"
#include "../grades.h"
#include "../ipc_wrapper.h"
#include "../linked_list.h"
#include "../logger.h"

volatile sig_atomic_t signalled = 0;

int main(int argc, char** argv) {
  struct DeanArguments arguments = initial_dean();
  FILE* grades_file;

  // Shared memory
  int dean_shmid = get_dean_shmid();
  int* k = (int*)shmat(dean_shmid, NULL, 0);

  // Semaphores
  int semaphore_id = get_semid();

  // Message queue
  struct Message message;
  int board_a = get_board_a_msgqid();
  int board_b = get_board_b_msgqid();

  // Linked list
  struct Node* head = NULL;

  srand(getpid());

  if (!parse_dean(argc, argv, &arguments)) {
    perror("Dean error. Failed to parse arguments");
    return EXIT_FAILURE;
  }

  if (!validate_dean(arguments)) {
    errno = EINVAL;
    perror("Dean error. Failed to validate arguments");
    return EXIT_FAILURE;
  }

  if (!attach_handler()) {
    perror("Dean error. Failed to attach signal handler");
    return EXIT_FAILURE;
  }

  if (dean_shmid == -1 || *k == -1) {
    perror("Dean error. Failed to attach shared memory");
    return EXIT_FAILURE;
  }

  if (semaphore_id == -1) {
    perror("Dean error. Failed to connect to semaphores");
    return EXIT_FAILURE;
  }

  if (board_a == -1 || board_b == -1) {
    perror("Dean error. Failed to connect to message queue");
    return EXIT_FAILURE;
  }

  log_dean_spawned(arguments);

#ifndef NOSLEEP
  sleep(rand() % 10 + 1);
#endif

  *k = rand() % arguments.k;

  if (!sem_post(semaphore_id, DEAN_SEMAPHORE, 0)) {
    perror("Dean error. Semaphore failed");
    return EXIT_FAILURE;
  }

  logger(DEAN_PREFIX, "Exam is for major %d\n", *k);

  // Recieve grades from board A
  while (1) {
    if (msgrcv(board_a, &message, MESSAGE_SIZE, MESSAGE_SEND_TO_DEAN, 0) ==
        -1) {
      if (errno == EINTR)
        continue;
      else {
        perror("Dean error. Could not recieve a message from message queue");
        return EXIT_FAILURE;
      }
    }

    if (message.slot1 < 0) break;

    linked_list_add(&head, (int)message.slot1, (int)message.slot2,
                    message.slot3, -1);
  }

  // Recieve grades from board B
  while (1) {
    if (msgrcv(board_b, &message, MESSAGE_SIZE, MESSAGE_SEND_TO_DEAN, 0) ==
        -1) {
      if (errno == EINTR)
        continue;
      else {
        perror("Dean error. Could not recieve a message from message queue");
        return EXIT_FAILURE;
      }
    }

    if (message.slot1 < 0) break;

    struct Node* curr = head;
    while (curr != NULL) {
      if (curr->k == message.slot1 && curr->n == message.slot2) {
        curr->grade_b = message.slot3;
      }

      curr = curr->next;
    }
  }

  // Print the grades
  grades_file = fopen("grades.txt", "w");

  logger(DEAN_PREFIX, "\nGRADES\n");

  if (grades_file != NULL) fprintf(grades_file, "GRADES\n");

  struct Node* curr = head;
  while (curr != NULL) {
    if (curr->grade_b < 0) {
      logger(DEAN_PREFIX, "Student[%d,%d] A: %.1f B: NA Final grade: NA\n",
             curr->k, curr->n, curr->grade_a);

      if (grades_file == NULL) continue;

      fprintf(grades_file, "Student[%d,%d] A: %.1f B: NA Final grade: NA\n",
              curr->k, curr->n, curr->grade_a);
    } else {
      logger(DEAN_PREFIX, "Student[%d,%d] A: %.1f B: %.1f Final grade: %.1f\n",
             curr->k, curr->n, curr->grade_a, curr->grade_b,
             get_final_grade(curr->grade_a, curr->grade_b));

      if (grades_file == NULL) continue;

      fprintf(grades_file, "Student[%d,%d] A: %.1f B: %.1f Final grade: %.1f\n",
              curr->k, curr->n, curr->grade_a, curr->grade_b,
              get_final_grade(curr->grade_a, curr->grade_b));
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
  if (signal == SIGUSR1 && signalled == 0) {
    signalled = 1;
    logger(DEAN_PREFIX, "Dean sent evacuation signal\n");
    kill(0, SIGUSR1);
    kill(getppid(), SIGUSR1);
  }
}
