#include "ipc_wrapper.h"

#include <stdbool.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

bool create_all_semaphores() {
  remove_all_semaphores();

  FILE* sem_file = fopen(SEMAPHORES_FILE, "w");
  key_t key;
  int semid;

  if (sem_file == NULL || fclose(sem_file) != 0) return false;

  key = ftok(SEMAPHORES_FILE, PROJECT_ID);

  if (key == -1) return false;

  semid = semget(key, SEMAPHORES_AMOUNT,
      IPC_CREAT | IPC_EXCL | PERMISSIONS);

  if (semid == -1) return false;

  if (semctl(semid, END_SEMAPHORE, SETVAL, END_SEMAPHORE_VALUE) == -1)
    return false;

  if (semctl(semid, DEAN_SEMAPHORE, SETVAL, DEAN_SEMAPHORE_VALUE) == -1)
    return false;

  return true;
}

bool remove_all_semaphores() {
  int semid = get_semid();
  bool result = true;

  if (semid == -1) return false;

  result = result && semctl(semid, END_SEMAPHORE, IPC_RMID) != -1;
  result = result && semctl(semid, DEAN_SEMAPHORE, IPC_RMID) != -1;

  if (remove(SEMAPHORES_FILE) == -1) return false;

  return result;
}

int get_semid() {
  key_t key = ftok(SEMAPHORES_FILE, PROJECT_ID);
  int semid;

  if (key == -1) return -1;

  semid = semget(key, SEMAPHORES_AMOUNT, PERMISSIONS);

  return semid;
}

bool sem_wait(int semid, short semaphore, short flags) {
  struct sembuf operation = {semaphore, SEMAPHORE_WAIT, flags};

  return (semop(semid, &operation, 1) != -1);
}

bool sem_post(int semid, short semaphore, short flags) {
  struct sembuf operation = {semaphore, SEMAPHORE_POST, flags};

  return (semop(semid, &operation, 1) != -1);
}

bool create_all_shared_memory() {
  remove_all_shared_memory();

  FILE* shm_file = fopen(DEAN_SHARED_MEMORY_FILE, "w");
  key_t key;

  if (shm_file == NULL) return false;

  if (fclose(shm_file) != 0) return false;

  key = ftok(DEAN_SHARED_MEMORY_FILE, PROJECT_ID);

  if (key == -1) return false;

  return shmget(key, DEAN_SHARED_MEMORY_SIZE,
      IPC_CREAT | IPC_EXCL | PERMISSIONS) != -1;
}

bool remove_all_shared_memory() {
  int shmid = get_dean_shmid();
  bool result;

  if (shmid == -1) return false;

  result = shmctl(shmid, IPC_RMID, NULL) != -1;

  if (remove(DEAN_SHARED_MEMORY_FILE) == -1) return false;

  return result;
}

int get_dean_shmid() {
  int shmid = -1;
  key_t key = ftok(DEAN_SHARED_MEMORY_FILE, PROJECT_ID);

  if (key == -1) return shmid;

  shmid = shmget(key, DEAN_SHARED_MEMORY_SIZE, PERMISSIONS);

  return shmid;
}

bool create_all_message_queues() {
  remove_all_message_queues();

  FILE* msgq_file;
  key_t key;
  bool result = true;

  for (size_t i = 0; i < MESSAGE_QUEUE_FILES_SIZE; i++) {
    msgq_file = fopen(MESSAGE_QUEUE_FILES[i], "w");

    if (msgq_file == NULL || fclose(msgq_file) != 0) return false;

    key = ftok(MESSAGE_QUEUE_FILES[i], PROJECT_ID);

    if (key == -1) return false;

    result = result && msgget(key, IPC_CREAT | IPC_EXCL | PERMISSIONS) != -1;
  }

  return result;
}

bool remove_all_message_queues() {
  int msgqid = get_board_a_msgqid();
  bool result = true;

  if (msgqid == -1) return false;

  result = result && msgctl(msgqid, IPC_RMID, NULL) != -1;
  msgqid = get_board_b_msgqid();
  result = result && msgctl(msgqid, IPC_RMID, NULL) != -1;

  if (remove(BOARD_A_MESSAGE_QUEUE_FILE) == -1) return false;
  if (remove(BOARD_B_MESSAGE_QUEUE_FILE) == -1) return false;

  return result;
}

int get_board_a_msgqid() {
  int msgqid = -1;
  key_t key = ftok(BOARD_A_MESSAGE_QUEUE_FILE, PROJECT_ID);

  if (key == -1) return msgqid;

  return msgget(key, PERMISSIONS);
}

int get_board_b_msgqid() {
  int msgqid = -1;
  key_t key = ftok(BOARD_A_MESSAGE_QUEUE_FILE, PROJECT_ID);

  if (key == -1) return msgqid;

  return msgget(key, PERMISSIONS);
}
