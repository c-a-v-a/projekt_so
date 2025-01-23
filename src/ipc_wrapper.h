#ifndef IPC_WRAPPER_H
#define IPC_WRAPPER_H

#include <stdbool.h>
#include <stddef.h>

#define PERMISSIONS 0600
static const char PROJECT_ID = 'E';

static const char* SEMAPHORES_FILE = "./semaphores";
static const int SEMAPHORES_AMOUNT = 2

static const short SEMAPHORE_WAIT = -1;
static const short SEMAPHORE_POST = 1;
static const short SEMAPHORE_FLAGS = 0;

static const unsigned short END_SEMAPHORE = 0;
static const unsigned short DEAN_SEMAPHORE = 1;

static const unsigned short END_SEMAPHORE_VALUE = 0;
static const unsigned short DEAN_SEMAPHORE_VALUE = 0;

static const char* DEAN_SHARED_MEMORY_FILE = "./dean_shm";
static const size_t DEAN_SHARED_MEMORY_SIZE = sizeof(int);

static const char* BOARD_A_MESSAGE_QUEUE_FILE = "./board_a_msgq";
static const char* BOARD_B_MESSAGE_QUEUE_FILE = "./board_b_msgq";

static const long MESSAGE_EMPTY = 1;
static const long MESSAGE_QUESTIONS = 2;
static const long MESSAGE_ANSWERS = 3;
static const long MESSAGE_GRADE = 4;
static const long MESSAGE_RETAKER = 5;

static const size_t MESSAGE_SIZE = 16;

struct Message {
  long mtype;
  char slot1[4];
  char slot2[4];
  char slot3[4];
  char total[4];
};

bool create_all_semaphores();
bool remove_all_semaphores();

int get_semid();

bool sem_wait(int semid, short semaphore, short flags);
bool sem_post(int semid, short semaphore, short flags);

bool create_all_shared_memory();
bool remove_all_shared_memory();

int get_dean_shmid();

bool create_all_message_queue();
bool remove_all_message_queue();

iny get_board_a_msgid();
iny get_board_b_msgid();

#endif
