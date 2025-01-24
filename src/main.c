#include "main.h"

#include <errno.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "cli_parser.h"
#include "ipc_wrapper.h"
#include "logger.h"

volatile sig_atomic_t SIGNALED = 0;

int main(int argc, char** argv) {
  struct MainArguments arguments = initial_main();
  pid_t dean;
  pid_t boards[] = {-1, -1};
  pid_t student_leader = -1;  // pid for the student group
  int semaphore_id;

  srand(time(NULL));

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

  semaphore_id = get_semid();

  if (semaphore_id == -1) {
    perror("Main error. Failed to connect to semaphores");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  sem_wait(semaphore_id, LOGGER_SEMAPHORE, 0);
  if (!log_main_spawned(arguments)) {
    perror("Main error. Failed to log program state");
  }
  sem_post(semaphore_id, LOGGER_SEMAPHORE, 0);

  if (!dean_runner(arguments.k, &dean)) {
    perror("Main error. Failed to spawn dean process");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  if (!board_runner(arguments.ns, arguments.ns_len, boards)) {
    perror("Main error. Failed to spawn board process");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  if (!students_runner(arguments.k, arguments.ns, arguments.t,
                       &student_leader)) {
    perror("Main error. Failed to spawn student process");
    cleanup(&arguments);

    return EXIT_FAILURE;
  }

  sleep(5);
  printf("%d %d %d %d", dean, boards[0], boards[1], student_leader);

  cleanup(&arguments);

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

bool dean_runner(int k, pid_t* dean) {
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

  return true;
}

bool board_runner(int* ns, ssize_t ns_len, pid_t* boards) {
  for (size_t i = 0; i < BOARDS_LENGTH; i++) {
    const pid_t pid = fork();

    if (pid == -1) {
      return false;
    } else if (pid == 0) {
      const char b[2] = {BOARDS[i], '\0'};
      char* ns_str = int_arr_to_str(ns, ns_len);

      if (ns_str == NULL) {
        perror("Board error. Failed to create arguments");

        exit(EXIT_FAILURE);
      }

      if (execl("./bin/board", "board", "-b", b, "-ns", ns_str, NULL) == -1) {
        perror("Board error. Failed to run process");
        free(ns_str);

        exit(EXIT_FAILURE);
      }
    } else {
      boards[i] = pid;
    }
  }

  return true;
}

bool students_runner(int k, int* ns, int t, pid_t* student_leader) {
  for (int i = 0; i < k; i++) {
    const int n = ns[i];

    for (int j = 1; j <= n; j++) {
      const pid_t pid = fork();

      if (pid == -1) {
        return false;
      } else if (pid == 0) {
        char* k_str = int_to_str(i + 1);
        char* n_str = int_to_str(j);
        char* t_str = int_to_str(t);

        if (k_str == NULL || n_str == NULL || t_str == NULL) {
          if (k_str != NULL) free(k_str);

          if (n_str != NULL) free(n_str);

          if (t_str != NULL) free(t_str);

          perror("Student error. Failed to create arguments");

          exit(EXIT_FAILURE);
        }

        if (*student_leader == -1) *student_leader = getpid();

        if (setpgid(0, *student_leader) == -1) {
          perror("Student error. Failed to set pgid");
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
      } else if (*student_leader == -1) {
        *student_leader = getpid();
      }
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
  if (signal == SIGUSR1 && SIGNALED == 0) {
    printf("MAIN: SIGUSR1\n");

    SIGNALED = 1;
    kill(0, SIGUSR1);
  }
}

char* int_to_str(int x) {
  size_t str_size = (size_t)((ceil(log10(x)) + 2) * sizeof(char));
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
