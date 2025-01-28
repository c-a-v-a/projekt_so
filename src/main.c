#include "main.h"

#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "cli_parser.h"
#include "ipc_wrapper.h"
#include "logger.h"

volatile pid_t dean = -1;
volatile sig_atomic_t signalled = 0;

int all_students = 0;
pid_t* pgid;
int semaphore_id;
pthread_t waiter;

int main(int argc, char** argv) {
  // Command line arguments
  struct MainArguments arguments = initial_main();

  // Children pid's
  pid_t boards[] = {-1, -1};

  // Shared memory
  int pgid_shmid;

  srand(getpid());

  // Parse main arguments
  if (!parse_main(argc, argv, &arguments)) {
    perror("Main error. Failed to parse arguments");

    return EXIT_FAILURE;
  }

  if (!fill_main(&arguments)) {
    perror("Main error. Failed to fill missing arguments");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  if (!validate_main(arguments)) {
    errno = EINVAL;

    perror("Main error. Failed to validate arguments");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  if (!attach_handler()) {
    perror("Main error. Failed to register signal handler");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  // System V structures creation
  if (!create_all_semaphores()) {
    perror("Main error. Failed to create semaphores");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  if (!create_all_shared_memory()) {
    perror("Main error. Failed to create shared memory blocks");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  if (!create_all_message_queues()) {
    perror("Main error. Failed to create message queues");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  // Linking semaphore
  semaphore_id = get_semid();
  if (semaphore_id == -1) {
    perror("Main error. Failed to connect to semaphores");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  // Linking shared memory
  pgid_shmid = get_pgid_shmid();
  pgid = (pid_t*)shmat(pgid_shmid, NULL, 0);

  if (pgid_shmid == -1 || pgid == (pid_t*)-1) {
    perror("Main error. Failed to attach to shared memory");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  *pgid = 0;
  if (!sem_post(semaphore_id, PGID_SEMAPHORE, 0)) {
    perror("Main error. Semaphore failed");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  if (!log_main_spawned(arguments)) {
    perror("Main error. Failed to log program state");
  }

  for (ssize_t i = 0; i < arguments.ns_len; i++)
    all_students += arguments.ns[i];

  if (pthread_create(&waiter, NULL, student_waiter, NULL) != 0) {
    perror("Main error. Cannot create a thread");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  // Spawning children
  if (!dean_runner(arguments.k, &dean)) {
    perror("Main error. Failed to spawn dean process");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  if (!board_runner(boards)) {
    perror("Main error. Failed to spawn board process");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  if (!students_runner(arguments.k, arguments.ns, arguments.t)) {
    perror("Main error. Failed to spawn student process");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  if (pthread_join(waiter, NULL) != 0) {
    perror("Main error. Cannot join thread");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  while (waitpid(boards[0], NULL, 0) > 0) {
  }
  while (waitpid(boards[1], NULL, 0) > 0) {
  }
  while (waitpid(dean, NULL, 0) > 0) {
  }

  if (shmdt(pgid) == -1) {
    perror("Main error. Failed to detach shared memory");
  }
  cleanup(&arguments);

  while (wait(NULL) > 0);

  return EXIT_SUCCESS;
}

void cleanup(struct MainArguments* arguments) {
  free(arguments->ns);

  if (!remove_all_semaphores()) {
    perror("Main error. Failed to clean up semaphores");
  }

  if (!remove_all_shared_memory()) {
    perror("Main error. Failed to clean up shared memory");
  }

  if (!remove_all_message_queues()) {
    perror("Main error. Failed to clean up message queues");
  }
}

bool dean_runner(int k, volatile pid_t* dean) {
  const pid_t pid = fork();

  if (pid == -1) {
    return false;
  } else if (pid == 0) {
    char* k_str = int_to_str(k);

    if (k_str == NULL) {
      perror("Dean error. Failed to create arguments");

      exit(EXIT_FAILURE);
    }

    if (execl("./bin/dean", "dean", "-k", k_str, NULL) == -1) {
      perror("Dean error. Failed to run process");
      free(k_str);

      exit(EXIT_FAILURE);
    }
  } else {
    *dean = pid;
  }

#ifndef NOSLEEP
  sleep(1);
#endif

  return true;
}

bool board_runner(pid_t* boards) {
  for (ssize_t i = 0; i < BOARDS_LENGTH; i++) {
    const pid_t pid = fork();

    if (pid == -1) {
      return false;
    } else if (pid == 0) {
      const char b[2] = {BOARDS[i], '\0'};

      if (execl("./bin/board", "board", "-b", b, NULL) == -1) {
        perror("Board error. Failed to run process");

        exit(EXIT_FAILURE);
      }
    } else {
      boards[i] = pid;
    }

#ifndef NOSLEEP
    sleep(1);
#endif
  }

  return true;
}

bool students_runner(int k, int* ns, int t) {
  for (int i = 0; i < k && signalled == 0; i++) {
    const int n = ns[i];

    for (int j = 0; j < n && signalled == 0; j++) {
      const pid_t pid = fork();

      if (pid == -1) {
        return false;
      } else if (pid == 0) {
        char* k_str = int_to_str(i);
        char* n_str = int_to_str(j);
        char* t_str = int_to_str(t);

        if (k_str == NULL || n_str == NULL || t_str == NULL) {
          if (k_str != NULL) free(k_str);

          if (n_str != NULL) free(n_str);

          if (t_str != NULL) free(t_str);

          perror("Student error. Failed to create arguments");

          exit(EXIT_FAILURE);
        }

        if (execl("./bin/student", "student", "-k", k_str, "-n", n_str, "-t",
                  t_str, NULL) == -1) {
          perror("Student error. Failed to run process");
          free(k_str);
          free(n_str);
          free(t_str);

          exit(EXIT_FAILURE);
        }
      }

#ifndef NOSLEEP
      usleep(rand() % 500000 + 100000);
#endif
    }
  }

  return true;
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
  if (signal == SIGUSR1 && dean != -1 && signalled == 0) {
    kill(dean, SIGUSR1);
    signalled = 1;
    pthread_cancel(waiter);
    if (!sem_post(semaphore_id, END_SEMAPHORE, 0)) {
      perror("Main error. Semaphore failure");
    }
  }
}

char* int_to_str(int x) {
  size_t str_size = (size_t)((ceil(log10(x + 1)) + 2) * sizeof(char));
  char* str = malloc(str_size);

  if (str == NULL) return str;

  if (sprintf(str, "%d", x) < 0) {
    // Should never happen
    free(str);
    return NULL;
  }

  return str;
}

char* int_arr_to_str(int* xs, ssize_t n) {
  size_t str_size = 0;
  char* str;

  if (n < 1) return NULL;

  for (ssize_t i = 0; i < n; i++)
    str_size += (size_t)((ceil(log10(xs[i])) + 2) * sizeof(char));

  str = malloc(str_size);

  if (str == NULL) return str;

  for (ssize_t i = 0; i < n; i++) {
    char* s = int_to_str(xs[i]);
    strcat(str, s);
    strcat(str, ",");
    free(s);
  }

  str[str_size - 1] = '\0';

  return str;
}

void* student_waiter() {
  int student_count = 0;
  printf("Hello from thread\n");

  while (1) {
    if (!sem_wait(semaphore_id, PGID_SEMAPHORE, 0)) {
      perror("Main error. Semaphore failure");
    }

    if (*pgid != 0) {
      sem_post(semaphore_id, PGID_SEMAPHORE, 0);
      break;
    }

    if (!sem_post(semaphore_id, PGID_SEMAPHORE, 0)) {
      perror("Main error. Semaphore failure");
    }
  }

  while (student_count < all_students) {
    if (wait(NULL) > 0) student_count++;
  }

  if (!sem_post(semaphore_id, END_SEMAPHORE, 0)) {
    perror("Main error. Semaphore failure");
  }

  return NULL;
}
