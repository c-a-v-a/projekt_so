#include "ipc_wrapper.h"

#include <stdbool.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>

bool create_all_semaphores() {
  remove_all_semaphores();

  FILE* sem_file = fopen(SEMAPHORE_SET, "w");
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
  key_t key = ftok(SEMAPHORES_SET, PROJECT_ID);
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
  remove_all_shm();

  FILE* shm_file = fopen(SHM_FILES[i], "w");
  key_t key;

  if (shm_file == NULL) return false;

  if (fclose(shm_file) != 0) return false;

  key = ftok(SHM_FILES[i], PROJECT_ID);

  if (key == -1) return false;

  return shmget(key, SHM_SIZES[i], IPC_CREAT | IPC_EXCL | SEMAPHORE_PERMISSIONS) != -1;
}

bool remove_all_shared_memory() {
  int shmid = get_dean_shmid();
  bool result;

  if (shmid == -1) return false;

  result = shmctl(shmid, IPC_RMID, NULL) != -1;

  if (remove(DEAN_SHARED_MEMORY_SIZE) == -1) return false;

  return result;
}

int get_dean_shmid() {
  int shmid = -1;
  key_t key = ftok(DEAN_SHARED_MEMORY, PROJECT_ID);

  if (key == -1) return shmid;

  shmid = shmget(key, DEAN_SHARED_MEMORY_SIZE, SEMAPHORE_PERMISSIONS);

  return shmid;
}
