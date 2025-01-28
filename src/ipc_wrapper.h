/**
 * @file ipc_wrapper.h
 * @author Filip Cebula
 * @date 23/01/2025
 * @brief Module that provides api for System V semaphores, shared memory and
 * message queues systems, that are used in this project.
 */

#ifndef IPC_WRAPPER_H
#define IPC_WRAPPER_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

/**
 * @brief Minimal possible permissions for ICP files, so that
 * semaphores, shared memory and message queues work as expected.
 */
#define PERMISSIONS 0600

/**
 * @brief Project id for IPC keys system.
 */
static const char PROJECT_ID = 'E';

/**
 * @brief Files to which all of our semaphores will be linked to.
 */
static const char SEMAPHORES_FILE[] = "./semaphores";

/**
 * @brief Amount of semaphores in the *SEMAPHORES_FILE*.
 */
static const int SEMAPHORES_AMOUNT = 7;

/**
 * @brief Constants providing values used in constructing *sembuf* operation
 * structure.
 */
static const short SEMAPHORE_WAIT = -1;
static const short SEMAPHORE_POST = 1;
static const short SEMAPHORE_FLAGS = 0;

/**
 * @brief ID of semaphores in semaphores file.
 */
static const unsigned short END_SEMAPHORE = 0;
static const unsigned short DEAN_SEMAPHORE = 1;
static const unsigned short LOGGER_SEMAPHORE = 2;
static const unsigned short BOARD_ROOM_A_SEMAPHORE = 3;
static const unsigned short BOARD_ROOM_B_SEMAPHORE = 4;
static const unsigned short BOARD_A_SEMAPHORE = 5;
static const unsigned short BOARD_B_SEMAPHORE = 6;

/**
 * @brief Initial value of the semaphores.
 */
static const unsigned short END_SEMAPHORE_VALUE = 0;
static const unsigned short DEAN_SEMAPHORE_VALUE = 0;
static const unsigned short LOGGER_SEMAPHORE_VALUE = 1;
static const unsigned short BOARD_ROOM_A_SEMAPHORE_VALUE = 3;
static const unsigned short BOARD_ROOM_B_SEMAPHORE_VALUE = 3;
static const unsigned short BOARD_A_SEMAPHORE_VALUE = 1;
static const unsigned short BOARD_B_SEMAPHORE_VALUE = 1;

/**
 * @brief Files to which shared memory will be linked to.
 */
static const char DEAN_SHARED_MEMORY_FILE[] = "./dean_shm";

/**
 * @brief Size of shared memory.
 */
static const size_t DEAN_SHARED_MEMORY_SIZE = sizeof(int);

/**
 * @brief Files to which message queues will be linked to.
 */
static const char BOARD_A_MESSAGE_QUEUE_FILE[] = "./board_a_msgq";
static const char BOARD_B_MESSAGE_QUEUE_FILE[] = "./board_b_msgq";
static const char MESSAGE_QUEUE_FILES[][15] = {"./board_a_msgq",
                                               "./board_b_msgq"};
static const size_t MESSAGE_QUEUE_FILES_SIZE = 2;

/**
 * @brief Types of message queue messages.
 */
#define MESSAGE_ASK 1
#define MESSAGE_QUESTIONS 2
#define MESSAGE_ANSWERS 3
#define MESSAGE_GRADE 4
#define MESSAGE_ACCEPT 5
#define MESSAGE_SEND_TO_DEAN 6

/**
 * @brief Type for the messages that will be send over the message queue.
 */
struct Message {
  long mtype; /**< Type of the message */

  /**
   * Since message will be send only to and from *board* process, each slot is
   * used by one thread of the board process.
   */
  float slot1;
  float slot2;
  float slot3;

  /**
   * Field used for final grade - that is an average of 3 grades from slot 1, 2
   * and 3 - and student grade if student happes to retake the exam.
   */
  float total;
};

/**
 * @brief Size of the message structure (withour message type field).
 *
 * @see Message
 */
static const size_t MESSAGE_SIZE = sizeof(struct Message) - sizeof(long);

/**
 * @brief Creates all semaphores that will be used in our system. It also
 * removes residuals before attempting to create new semaphores.
 *
 * @return Wheather the operation failed or succeeded.
 */
bool create_all_semaphores();

/**
 * @brief Unlinks the semaphores from files and removes the files from the
 * system.
 *
 * @return Wheather the operation failed or succeeded.
 */
bool remove_all_semaphores();

/**
 * Get id of the semaphore file.
 *
 * @return Semaphore file id or -1 if error occured.
 */
int get_semid();

/**
 * @brief Define wait and post operation on semaphores.
 *
 * @return Wheather the operation failed of succeeded.
 */
bool sem_wait(int semid, short semaphore, short flags);
bool sem_post(int semid, short semaphore, short flags);

/**
 * @brief Creates all shared memories that will be used in our system. It also
 * removes residuals before attempting to create new semaphores.
 *
 * @return Wheather the operation failed or succeeded.
 */
bool create_all_shared_memory();

/**
 * @breif Unlinks the shared memory from files and removes the files from the
 * system.
 *
 * @return Wheather the operation failed or succeeded.
 */
bool remove_all_shared_memory();

/**
 * @brief Gets an id of shared memory.
 *
 * @return Shared memory id or -1 if error occured.
 */
int get_dean_shmid();

/**
 * @brief Creates all message queues that will be used in our system. It also
 * removes residuals before attempting to create new semaphores.
 *
 * @return Wheather the operation failed or succeeded.
 */
bool create_all_message_queues();

/**
 * @brief Unlinks the message queues from files and removes the files from the
 * system.
 *
 * @return Wheather the operation failed or succeeded.
 */
bool remove_all_message_queues();

/**
 * @brief Gets an id of message queue.
 *
 * @return Message queue id or -1 if error occured.
 */
int get_board_a_msgqid();
int get_board_b_msgqid();

#endif
