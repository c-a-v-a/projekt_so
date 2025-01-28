#include "student.h"

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
#include "../logger.h"

int main(int argc, char** argv) {
  struct StudentArguments arguments = initial_student();

  // Semaphores
  int semaphore_id = get_semid();

  // Shared memory
  int dean_shmid = get_dean_shmid();
  int* k;

  // Message queue
  int msgqid;

  float result;
  bool retaker = false;
  float grade = -1;

  srand(getpid());

  // Decide if student is a retaker
  retaker = rand() % 100 < RETAKER_PROBABILITY;

  if (retaker) grade = GRADES[rand() % 5 + 1];

  if (!parse_student(argc, argv, &arguments)) {
    perror("Student error. Failed to parse arguments");
    return EXIT_FAILURE;
  }

  if (!validate_student(arguments)) {
    errno = EINVAL;
    perror("Student error. Failed to validate arguments");
    return EXIT_FAILURE;
  }

  if (!attach_handler()) {
    perror("Student error. Failed to attach signal handler");
    return EXIT_FAILURE;
  }

  if (dean_shmid == -1) {
    perror("Student error. Failed to connect to shared memory");
    return EXIT_FAILURE;
  }

  if (semaphore_id == -1) {
    perror("Student error. Failed to connect to semaphores");
    return EXIT_FAILURE;
  }

  log_student_spawned(arguments);

  // Wait for dean major message
  if (!sem_wait(semaphore_id, DEAN_SEMAPHORE, 0)) {
    perror("Student error. Semaphore failed");
    return EXIT_FAILURE;
  }

  k = (int*)shmat(dean_shmid, NULL, 0);
  if (*k == -1) {
    perror("Student error. Failed to attach shared memory");
    return EXIT_FAILURE;
  }

  if (!sem_post(semaphore_id, DEAN_SEMAPHORE, 0)) {
    perror("Student error. Semaphore failed");
    return EXIT_FAILURE;
  }

  // Exit if wrong major
  if (*k != arguments.k) {
    shmdt(k);
    logger(STUDENT_PREFIX, "Student[%d,%d] exits. Wrong major\n", arguments.k,
           arguments.n);
    exit(EXIT_SUCCESS);
  }
  shmdt(k);

  // BOARD A
  if ((msgqid = get_board_a_msgqid()) == -1) {
    perror("Student error. Failed to connect to message queue");
    return EXIT_FAILURE;
  }

  result =
      board_action(semaphore_id, BOARD_ROOM_A_SEMAPHORE, BOARD_A_SEMAPHORE,
                   msgqid, BOARD_ROOM_A_PREFIX, retaker, arguments, grade, 'A');

  if (result < 0) {
    return EXIT_FAILURE;
  } else if (result < 3) {
    logger(STUDENT_PREFIX, "Student[%d,%d] finished exam (didn't pass)\n",
           arguments.k, arguments.n);

    return EXIT_SUCCESS;
  }

  // BOARD B
  if ((msgqid = get_board_b_msgqid()) == -1) {
    perror("Student error. Failed to connect to message queue");
    return EXIT_FAILURE;
  }

  result = board_action(semaphore_id, BOARD_ROOM_B_SEMAPHORE, BOARD_B_SEMAPHORE,
                        msgqid, BOARD_ROOM_B_PREFIX, false, arguments, -1, 'B');

  if (result < 0) {
    return EXIT_FAILURE;
  } else if (result < 3) {
    logger(STUDENT_PREFIX, "Student[%d,%d] finished exam (didn't pass)\n",
           arguments.k, arguments.n);

    return EXIT_SUCCESS;
  }

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
    exit(EXIT_SUCCESS);
  }
}

float board_action(int semid, int sem_room, int sem, int msgqid,
                   const char* prefix, bool retaker,
                   struct StudentArguments args, float grade, char board_name) {
  struct Message message;

  // Go in the board room
  if (!sem_wait(semid, sem_room, 0)) {
    perror("Student error. Semaphore failed");
    return -1.;
  }

  logger(prefix, "Student[%d,%d] entered board %c room\n", args.k, args.n,
         board_name);

  // Go in front of the board
  if (!sem_wait(semid, sem, 0)) {
    perror("Student error. Semaphore failed");
    return -1.;
  }

  if (retaker) {
    // Confirm grade
    message.slot1 = args.k;
    message.slot2 = args.n;
    message.slot3 = grade;
    message.mtype = MESSAGE_ACCEPT;
    if (msgsnd(msgqid, &message, MESSAGE_SIZE, 0) == -1) {
      perror("Student error. Failed to send a message");
      return -1.;
    }

    logger(prefix, "Student[%d,%d] retakes the exam with grade %.1f\n", args.k,
           args.n, grade);

    if (!sem_post(semid, sem, 0)) {
      perror("Student error. Semaphore failed");
      return -1.;
    }

    if (!sem_post(semid, sem_room, 0)) {
      perror("Student error. Semaphore failed");
      return -1.;
    }

    return grade;
  } else {
    // Get questions
    message.mtype = MESSAGE_ASK;
    if (msgsnd(msgqid, &message, MESSAGE_SIZE, 0)) {
      perror("Student error. Failed to send a message");
      return -1;
    }

    if (msgrcv(msgqid, &message, MESSAGE_SIZE, MESSAGE_QUESTIONS, 0) == -1) {
      perror("Student error. Failed to recieve a message");
      return -1;
    }

    logger(prefix, "Student[%d,%d] recieved questions %.1f,%.1f,%.1f\n", args.k,
           args.n, message.slot1, message.slot2, message.slot3);

    // Go back
    if (!sem_post(semid, sem, 0)) {
      perror("Student error. Semaphore failed");
      return -1.;
    }

    // Think
    sleep(args.t);

    // Go in front of the board again
    if (!sem_wait(semid, sem, 0)) {
      perror("Student error. Semaphore failed");
      return -1.;
    }

    // Send answers
    message.mtype = MESSAGE_ANSWERS;
    if (msgsnd(msgqid, &message, MESSAGE_SIZE, 0) == -1) {
      perror("Student error. Failed to send a message");
      return -1;
    }

    // Get grades
    if (msgrcv(msgqid, &message, MESSAGE_SIZE, MESSAGE_GRADE, 0) == -1) {
      perror("Student error. Failed to recieve a message");
      return -1;
    }

    logger(prefix, "Student[%d,%d] recieved grades %.1f,%.1f,%.1f total %.1f\n",
           args.k, args.n, message.slot1, message.slot2, message.slot3,
           message.total);

    message.slot1 = args.k;
    message.slot2 = args.n;
    message.slot3 = message.total;
    message.mtype = MESSAGE_ACCEPT;
    if (msgsnd(msgqid, &message, MESSAGE_SIZE, 0) == -1) {
      perror("Student error. Failed to send a message");
      return -1;
    }

    if (!sem_post(semid, sem, 0)) {
      perror("Student error. Semaphore failed");
      return -1.;
    }

    if (!sem_post(semid, sem_room, 0)) {
      perror("Student error. Semaphore failed");
      return -1.;
    }

    return message.total;
  }
}
