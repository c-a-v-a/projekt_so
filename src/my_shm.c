#include "my_shm.h"

#include <stdbool.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "my_semaphores.h"

bool create_all_shm() {
  remove_all_shm();

  FILE* shm_file;
  int shmid;
  key_t key;

  for (int i = 0; i < SHM_ARR_SIZE; i++) {
    shm_file = fopen(SHM_FILES[i], "w");

    if (shm_file == NULL) return false;

    if (fclose(shm_file) != 0) return false;

    key = ftok(SHM_FILES[i], PROJECT_ID);

    if (key == -1) return false;

    shmid =
        shmget(key, SHM_SIZES[i], IPC_CREAT | IPC_EXCL | SEMAPHORE_PERMISSIONS);

    if (shmid == -1) return false;
  }

  return true;
}

bool remove_all_shm() {
  int shmid;
  key_t key;
  bool result = true;

  for (int i = 0; i < SHM_ARR_SIZE; i++) {
    key = ftok(SHM_FILES[i], PROJECT_ID);

    if (key == -1) return false;

    shmid = shmget(key, SHM_SIZES[i], SEMAPHORE_PERMISSIONS);

    if (shmid == -1) return false;

    result = result && shmctl(shmid, IPC_RMID, NULL) != -1;

    if (remove(SHM_FILES[i]) == -1) return false;
  }

  return result;
}

int get_dean_shmid() {
  int shmid = -1;
  key_t key = ftok(DEAN_SHARED_MEMORY, PROJECT_ID);

  if (key == -1) return shmid;

  shmid = shmget(key, DEAN_SHARED_MEMORY_SIZE, SEMAPHORE_PERMISSIONS);

  return shmid;
}

int get_board_a_shmid() {
  int shmid = -1;
  key_t key = ftok(BOARD_A_SHARED_MEMORY, PROJECT_ID);

  if (key == -1) return shmid;

  shmid = shmget(key, BOARD_A_SHARED_MEMORY_SIZE, SEMAPHORE_PERMISSIONS);

  return shmid;
}

int get_board_b_shmid() {
  int shmid = -1;
  key_t key = ftok(BOARD_B_SHARED_MEMORY, PROJECT_ID);

  if (key == -1) return shmid;

  shmid = shmget(key, BOARD_B_SHARED_MEMORY_SIZE, SEMAPHORE_PERMISSIONS);

  return shmid;
}
