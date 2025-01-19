#ifndef MY_SEMAPHORES_H
#define MY_SEMAPHORES_H

#include <stdbool.h>
#include <sys/sem.h>

#define SEMAPHORE_PERMISSIONS 0600

static const char PROJECT_ID = 'E';

static const char* SEMAPHORE_SET = "./semaphores";
static const int SEMAPHORE_AMOUNT = 2;

static const short SEMAPHORE_WAIT = -1;
static const short SEMAPHORE_POST = 1;
static const short SEMAPHORE_FLAGS = 0;

static const int END_SEMAPHORE = 0;
static const int DEAN_SEMAPHORE = 0;

static const int END_SEMAPHORE_VALUE = 0;
static const int DEAN_SEMAPHORE_VALUE = 0;

bool create_all_semaphores();
bool remove_all_semaphores();

int get_semid();

bool sem_wait(int semid, int semaphore);
bool sem_post(int semid, int semaphore);

#endif
