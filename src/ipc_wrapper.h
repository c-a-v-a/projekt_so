/**
 * @file ipc_wrapper.h
 * @brief Module that provides api for System V semaphores, shared memory and
 * message queues systems, that are used in this project.
 *
 * @author Filip Cebula
 * @date 23/01/2025
 */

#ifndef IPC_WRAPPER_H
#define IPC_WRAPPER_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

/**
 * Minimal possible permissions for ICP files, so that
 * semaphores, shared memory and message queues work as expected.
 */
#define PERMISSIONS 0600

/**
 * Project id for IPC keys system.
 */
static const char PROJECT_ID = 'E';

/**
 * Files to which all of our semaphores will be linked to.
 */
static const char SEMAPHORES_FILE[] = "./semaphores";

/**
 * Amount of semaphores in the *SEMAPHORES_FILE*.
 */
static const int SEMAPHORES_AMOUNT = 6;

/**
 * Constants providing values used in constructing *sembuf* operation structure.
 */
static const short SEMAPHORE_WAIT = -1;
static const short SEMAPHORE_POST = 1;
static const short SEMAPHORE_FLAGS = 0;

/**
 * ID of semaphores in semaphores file.
 */
static const unsigned short END_SEMAPHORE = 0;
static const unsigned short DEAN_SEMAPHORE = 1;
static const unsigned short LOGGER_SEMAPHORE = 2;
static const unsigned short PGID_SEMAPHORE = 3;
static const unsigned short BOARD_ROOM_A_SEMAPHORE = 4;
static const unsigned short BOARD_ROOM_B_SEMAPHORE = 5;

/**
 * Initial value of the semaphores.
 */
static const unsigned short END_SEMAPHORE_VALUE = 0;
static const unsigned short DEAN_SEMAPHORE_VALUE = 0;
static const unsigned short LOGGER_SEMAPHORE_VALUE = 1;
static const unsigned short PGID_SEMAPHORE_VALUE = 0;
static const unsigned short BOARD_ROOM_A_SEMAPHORE_VALUE = 3;
static const unsigned short BOARD_ROOM_B_SEMAPHORE_VALUE = 3;

/**
 * Files to which shared memory will be linked to.
 */
static const char DEAN_SHARED_MEMORY_FILE[] = "./dean_shm";
static const char PGID_SHARED_MEMORY_FILE[] = "./pgid_shm";
static const char SHARED_MEMORY_FILES[][11] = {"./dean_shm", "./pgid_shm"};
static const size_t SHARED_MEMORY_FILES_SIZE = 2;

/**
 * Size of shared memory.
 */
static const size_t DEAN_SHARED_MEMORY_SIZE = sizeof(int);
static const size_t PGID_SHARED_MEMORY_SIZE = sizeof(pid_t);
static const size_t SHARED_MEMORY_SIZES[] = {sizeof(int), sizeof(pid_t)};

/**
 * Files to which message queues will be linked to.
 */
static const char BOARD_A_MESSAGE_QUEUE_FILE[] = "./board_a_msgq";
static const char BOARD_B_MESSAGE_QUEUE_FILE[] = "./board_b_msgq";
static const char MESSAGE_QUEUE_FILES[][15] = {"./board_a_msgq",
                                               "./board_b_msgq"};
static const size_t MESSAGE_QUEUE_FILES_SIZE = 2;

/**
 * Types of message queue messages.
 */
static const long MESSAGE_EMPTY = 1;
static const long MESSAGE_QUESTIONS = 2;
static const long MESSAGE_ANSWERS = 3;
static const long MESSAGE_GRADE = 4;
static const long MESSAGE_RETAKER = 5;
static const long MESSAGE_SEND_TO_DEAN = 6;

/**
 * Size of the message structure (withour message type field).
 *
 * @see Message
 */
static const size_t MESSAGE_SIZE = 16;

/**
 * Type for the messages that will be send over the message queue.
 */
struct Message {
  long mtype; /**< Type of the message */

  /**
   * Since message will be send only to and from *board* process, each slot is
   * used by one thread of the board process.
   */
  char slot1[4];
  char slot2[4];
  char slot3[4];

  /**
   * Field used for final grade - that is an average of 3 grades from slot 1, 2
   * and 3 - and student grade if student happes to retake the exam.
   */
  char total[4];
};

/**
 * Creates all semaphores that will be used in our system. It also removes
 * residuals before attempting to create new semaphores.
 *
 * @return Wheather the operation failed or succeded.
 */
bool create_all_semaphores();

/**
 * Unlinks the semaphores from files and removes the files from the system.
 *
 * @return Wheather the operation failed or succeded.
 */
bool remove_all_semaphores();

/**
 * Get id of the semaphore file.
 *
 * @return Semaphore file id od -1 if error occured.
 */
int get_semid();

/**
 * Define wait and post operation on semaphores.
 *
 * @return Wheather the operation failed of succeded.
 */
bool sem_wait(int semid, short semaphore, short flags);
bool sem_post(int semid, short semaphore, short flags);

/**
 * Creates all shared memories that will be used in our system. It also removes
 * residuals before attempting to create new semaphores.
 *
 * @return Wheather the operation failed or succeded.
 */
bool create_all_shared_memory();

/**
 * Unlinks the shared memory from files and removes the files from the system.
 *
 * @return Wheather the operation failed or succeded.
 */
bool remove_all_shared_memory();

/**
 * Gets an id of shared memory.
 *
 * @return Shared memory id or -1 if error occured.
 */
int get_dean_shmid();
int get_pgid_shmid();

/**
 * Creates all message queues that will be used in our system. It also removes
 * residuals before attempting to create new semaphores.
 *
 * @return Wheather the operation failed or succeded.
 */
bool create_all_message_queues();

/**
 * Unlinks the message queues from files and removes the files from the system.
 *
 * @return Wheather the operation failed or succeded.
 */
bool remove_all_message_queues();

/**
 * Gets an id of message queue.
 *
 * @return Message queue id or -1 if error occured.
 */
int get_board_a_msgqid();
int get_board_b_msgqid();

#endif
