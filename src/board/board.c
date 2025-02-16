#include "board.h"

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>

#include "../cli_parser.h"
#include "../grades.h"
#include "../ipc_wrapper.h"
#include "../linked_list.h"
#include "../logger.h"

pthread_mutex_t board_member_mutex;

int main(int argc, char** argv) {
  struct BoardArguments arguments = initial_board();

  // Threads
  pthread_t t1, t2, t3;
  int result = 0;

  // Message queue
  struct Message message;
  int msgqid;

  // Semaphores
  int semaphore_id = get_semid();

  // Linked list
  struct Node* head = NULL;
  struct Node* curr;

  srand(getpid());

  if (!parse_board(argc, argv, &arguments)) {
    perror("Board error. Failed to parse arguments");
    return EXIT_FAILURE;
  }

  if (!validate_board(arguments)) {
    errno = EINVAL;
    perror("Board error. Failed to validate arguments");
    return EXIT_FAILURE;
  }

  if (!attach_handler()) {
    perror("Board error. Failed to attach signal handler");
    return EXIT_FAILURE;
  }

  if (semaphore_id == -1) {
    perror("Board error. Failed to connect to semaphores");
    return EXIT_FAILURE;
  }

  if (arguments.board_name == 'A') {
    msgqid = get_board_a_msgqid();
  } else {
    msgqid = get_board_b_msgqid();
  }

  if (msgqid == -1) {
    perror("Board error. Failed to connect to message queue");
    return EXIT_FAILURE;
  }

  if (pthread_mutex_init(&board_member_mutex, NULL) != 0) {
    perror("Board error. Failed to initialize mutex");
    return EXIT_FAILURE;
  }

  log_board_spawned(arguments);

  while (1) {
    if (sem_wait(semaphore_id, END_SEMAPHORE, IPC_NOWAIT)) {
      sem_post(semaphore_id, END_SEMAPHORE, IPC_NOWAIT);
      break;
    }

    if (msgrcv(msgqid, &message, MESSAGE_SIZE, MESSAGE_ASK, IPC_NOWAIT) == -1) {
      if (errno != ENOMSG) {
        perror("Board error. Failed to recieve messages from message queue");
        return EXIT_FAILURE;
      }
    } else {
      message.mtype = MESSAGE_QUESTIONS;
      message.slot1 = -1.;
      message.slot2 = -1.;
      message.slot3 = -1.;

      result += pthread_create(&t1, NULL, prepare_questions, (void*)&message);
      result += pthread_create(&t2, NULL, prepare_questions, (void*)&message);
      result += pthread_create(&t3, NULL, prepare_questions, (void*)&message);

      if (result != 0) {
        perror("Board error. Failed to create threads");
        return EXIT_FAILURE;
      }

      result += pthread_join(t1, NULL);
      result += pthread_join(t2, NULL);
      result += pthread_join(t3, NULL);

      if (result != 0) {
        perror("Board error. Failed to join threads");
        return EXIT_FAILURE;
      }

      if (msgsnd(msgqid, &message, MESSAGE_SIZE, 0) == -1) {
        perror("Board error. Failed to send message");
        return EXIT_FAILURE;
      }

      continue;
    }

    if (msgrcv(msgqid, &message, MESSAGE_SIZE, MESSAGE_ANSWERS, IPC_NOWAIT) ==
        -1) {
      if (errno != ENOMSG) {
        perror("Board error. Failed to recieve messages from message queue");
        return EXIT_FAILURE;
      }
    } else {
      message.mtype = MESSAGE_GRADE;
      message.slot1 = -1.;
      message.slot2 = -1.;
      message.slot3 = -1.;

      result += pthread_create(&t1, NULL, grade, (void*)&message);
      result += pthread_create(&t2, NULL, grade, (void*)&message);
      result += pthread_create(&t3, NULL, grade, (void*)&message);

      if (result != 0) {
        perror("Board error. Failed to create threads");
        return EXIT_FAILURE;
      }

      result += pthread_join(t1, NULL);
      result += pthread_join(t2, NULL);
      result += pthread_join(t3, NULL);

      if (result != 0) {
        perror("Board error. Failed to join threads");
        return EXIT_FAILURE;
      }

      if (msgsnd(msgqid, &message, MESSAGE_SIZE, 0) == -1) {
        perror("Board error. Failed to send message");
        return EXIT_FAILURE;
      }

      continue;
    }

    if (msgrcv(msgqid, &message, MESSAGE_SIZE, MESSAGE_ACCEPT, IPC_NOWAIT) ==
        -1) {
      if (errno != ENOMSG) {
        perror("Board error. Failed to recieve messages from message queue");
        return EXIT_FAILURE;
      }
    } else {
      linked_list_add(&head, (int)message.slot1, (int)message.slot2,
                      message.slot3, -1.);

      continue;
    }
  }

  curr = head;
  while (curr != NULL) {
    message.mtype = MESSAGE_SEND_TO_DEAN;
    message.slot1 = (float)curr->k;
    message.slot2 = (float)curr->n;
    message.slot3 = curr->grade_a;

    if (msgsnd(msgqid, &message, MESSAGE_SIZE, 0) == -1) {
      perror("Board error. Failed to send message");
      return EXIT_FAILURE;
    }

    curr = curr->next;
  }

  message.mtype = MESSAGE_SEND_TO_DEAN;
  message.slot1 = -1.;
  if (msgsnd(msgqid, &message, MESSAGE_SIZE, 0) == -1) {
    perror("Board error. Failed to send message");
    return EXIT_FAILURE;
  }

  linked_list_free(head);

  return EXIT_SUCCESS;
}

void* prepare_questions(void* arg) {
  struct Message* message = (struct Message*)arg;

  pthread_mutex_lock(&board_member_mutex);

  if (message->slot1 <= 0) {
    message->slot1 = (float)(rand() % 10 + 1);
  } else if (message->slot2 <= 0) {
    message->slot2 = (float)(rand() % 10 + 1);
  } else if (message->slot3 <= 0) {
    message->slot3 = (float)(rand() % 10 + 1);
  }

  pthread_mutex_unlock(&board_member_mutex);

  return NULL;
}

void* grade(void* arg) {
  struct Message* message = (struct Message*)arg;

  pthread_mutex_lock(&board_member_mutex);

  if (message->slot1 <= 0) {
    message->slot1 = get_random_grade();
  } else if (message->slot2 <= 0) {
    message->slot2 = get_random_grade();
  } else if (message->slot3 <= 0) {
    message->slot3 = get_random_grade();
  }

  if (message->slot1 > 0 && message->slot2 > 0 && message->slot3 > 0) {
    if (message->slot1 < 3 || message->slot2 < 3 || message->slot3 < 3) {
      message->total = 2.;
    } else {
      message->total =
          get_average(message->slot1, message->slot2, message->slot3);
    }
  }

  pthread_mutex_unlock(&board_member_mutex);

  return NULL;
}

bool attach_handler() {
  struct sigaction sa;

  sa.sa_handler = SIG_IGN;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);

  if (sigaction(SIGUSR1, &sa, NULL) == -1) return false;

  return true;
}
