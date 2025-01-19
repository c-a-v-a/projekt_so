#include "my_semaphores.h"

#include <stdbool.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>

bool create_all_semaphores() {
  // try to remove semaphores if some residuals stayed
  remove_all_semaphores();

  FILE* sem_file = fopen(SEMAPHORE_SET, "w");
  key_t key;
  int semid;

  if (sem_file == NULL) return false;

  if (fclose(sem_file) != 0) return false;

  key = ftok(SEMAPHORE_SET, PROJECT_ID);

  if (key == -1) return false;

  semid = semget(key, SEMAPHORE_AMOUNT,
                 IPC_CREAT | IPC_EXCL | SEMAPHORE_PERMISSIONS);

  if (semid == -1) return false;

  if (semctl(semid, END_SEMAPHORE, SETVAL, END_SEMAPHORE_VALUE) == -1)
    return false;

  if (semctl(semid, DEAN_SEMAPHORE, SETVAL, DEAN_SEMAPHORE_VALUE) == -1)
    return false;

  return true;
}

bool remove_all_semaphores() {
  key_t key = ftok(SEMAPHORE_SET, PROJECT_ID);
  int semid;
  bool result = true;

  if (key == -1) return false;

  semid = semget(key, SEMAPHORE_AMOUNT, SEMAPHORE_PERMISSIONS);

  if (semid == -1) return false;

  result = result && semctl(semid, END_SEMAPHORE, IPC_RMID) == -1;
  result = result && semctl(semid, DEAN_SEMAPHORE, IPC_RMID) == -1;

  if (remove(SEMAPHORE_SET) == -1) return false;

  return result;
}

int get_semid() {
  key_t key = ftok(SEMAPHORE_SET, PROJECT_ID);
  int semid;

  if (key == -1) return -1;

  semid = semget(key, SEMAPHORE_AMOUNT, SEMAPHORE_PERMISSIONS);

  if (semid == -1) return -1;

  return semid;
}

bool sem_wait(int semid, int semaphore) {
  struct sembuf operation = {semaphore, SEMAPHORE_WAIT, SEMAPHORE_FLAGS};

  if (semop(semid, &operation, 1) == -1) return false;

  return true;
}

bool sem_post(int semid, int semaphore) {
  struct sembuf operation = {semaphore, SEMAPHORE_POST, SEMAPHORE_FLAGS};

  if (semop(semid, &operation, 1) == -1) return false;

  return true;
}
