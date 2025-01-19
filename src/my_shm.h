#ifndef MY_SHM_H
#define MY_SHM_H

#include <stdbool.h>
#include <stddef.h>
// TODO: Move permissions and PROJECT_ID to defaults or other file
// TODO: Move semaphores and shared memory into sem and shm directories
// TODO: Abbreviate semaphores and shared memory and whatever else needs to
// be abbreviated

static const char* DEAN_SHARED_MEMORY = "./dean_shm";
static const char* BOARD_A_SHARED_MEMORY = "./board_a_shm";
static const char* BOARD_B_SHARED_MEMORY = "./board_b_shm";

static const size_t DEAN_SHARED_MEMORY_SIZE = sizeof(int);
// Change size when we will define struct for communication
static const size_t BOARD_A_SHARED_MEMORY_SIZE = sizeof(int);
static const size_t BOARD_B_SHARED_MEMORY_SIZE = sizeof(int);

static const char* SHM_FILES[] = {"./dean_shm", "./board_a_shm",
                                  "./board_b_shm"};
static const size_t SHM_SIZES[] = {sizeof(int), sizeof(int), sizeof(int)};
static const int SHM_ARR_SIZE = 3;

bool create_all_shm();
bool remove_all_shm();

int get_dean_shmid();
int get_board_a_shmid();
int get_board_b_shmid();

#endif
