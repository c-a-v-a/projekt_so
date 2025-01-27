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
#include "../linked_list.h"
#include "../logger.h"

pthread_mutex_t board_member_mutex;

int main(int argc, char** argv) {
  struct BoardArguments args = initial_board();
  pthread_t t1, t2, t3;
  int msgqid;
  int semaphore_id = get_semid();
  struct Message message;
  struct Node* head = NULL;

  setpgid(0, 0);
  srand(getpid());

  if (!parse_board(argc, argv, &args)) {
    perror("Board error. Failed to parse arguments");
    return EXIT_FAILURE;
  }

  if (!validate_board(args)) {
    errno = EINVAL;
    perror("Board error. Failed to validate arguments");
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

  pthread_mutex_init(&board_member_mutex, NULL);

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
        message.slot1 = -1.;
        message.slot2 = -1.;
        message.slot3 = -1.;

        pthread_create(&t1, NULL, prepare_questions, (void*)&message);
        pthread_create(&t2, NULL, prepare_questions, (void*)&message);
        pthread_create(&t3, NULL, prepare_questions, (void*)&message);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        pthread_join(t3, NULL);

        msgsnd(msgqid, &message, MESSAGE_SIZE, 0);

        break;
      case MESSAGE_ANSWERS:
        message.mtype = MESSAGE_GRADE;
        message.slot1 = -1.;
        message.slot2 = -1.;
        message.slot3 = -1.;

        pthread_create(&t1, NULL, grade, (void*)&message);
        pthread_create(&t2, NULL, grade, (void*)&message);
        pthread_create(&t3, NULL, grade, (void*)&message);

        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        pthread_join(t3, NULL);

        msgsnd(msgqid, &message, MESSAGE_SIZE, 0);

        break;
      case MESSAGE_RETAKER:
        linked_list_add(&head, (int)message.slot1, (int)message.slot2, message.slot3, -1.);

        break;
      default:
        errno = EINVAL;
        perror("Board error. Bad message type");
        break;
    }
  }
  
  struct Node* curr = head;
  while (curr != NULL) {
    message.mtype = MESSAGE_SEND_TO_DEAN;
    message.slot1 = (float)curr->k;
    message.slot2 = (float)curr->n;
    message.slot3 = curr->grade_a;
    msgsnd(msgqid, &message, MESSAGE_SIZE, 0);

    curr = curr->next;
  }

  message.mtype = MESSAGE_SEND_TO_DEAN;
  message.slot1 = -1.;
  msgsnd(msgqid, &message, MESSAGE_SIZE, 0);

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
      message->total = get_average(message->slot1, message->slot2, message->slot3);
    }
  }

  pthread_mutex_unlock(&board_member_mutex);

  return NULL;
}

float get_random_grade() {
  float grade = -1;
  int probability = rand() % 100 + 1;
  int i = 0;

  for (; i < GRADES_LEN; i++) {
    probability -= GRADES_PROBABILITY[i];
    grade = GRADES[i];

    if (probability <= 0) break;
  }
  
  return grade;
}

float get_average(float x, float y, float z) {
  float avg = (x + y + z) / 3;

  for (int i = 0; i < GRADES_LEN; i++) {
    if (GRADES[i] >= avg) return GRADES[i];
  }

  return -1;
}
