#include "main.h"

#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "cli_parser.h"
#include "ipc_wrapper.h"

#include "argprinter.h"
#include "defaults.h"
#include "str_creator.h"

// TODO: print to log file and not to stdout
// TODO: take care of zombies

volatile sig_atomic_t SIGNALED = 0;

int main(int argc, char** argv) {
  struct DeanArguments args = initial_dean();

  srand(time(NULL));

  if (setpgid(0, 0) == -1) {
    perror("Unable to create a pid group");
    return EXIT_FAILURE;
  }

  if (!handle_signal()) {
    perror("Unable to register signal handler");
    return EXIT_FAILURE;
  }

  if (!parse_dean(argc, argv, &args)) {
    perror("Unable to parse Dean arguments");
    return EXIT_FAILURE;
  }

  if (!fill_dean(&args)) {
    perror("Unable to parse Dean arguments");
    return EXIT_FAILURE;
  }

  if (!validate_dean(args)) {
    if (args.ns != NULL) free(args.ns);

    errno = EINVAL;
    perror("Invalid Dean arguments");

    return EXIT_FAILURE;
  }

  if (!create_all_semaphores()) {
    perror("Unable to create semaphores");

    return EXIT_FAILURE;
  }

  if (!create_all_shared_memory()) {
    perror("Unable to create shared memory blocks");

    return EXIT_FAILURE;
  }

  /*
  if (!dean_runner(args.k)) {
    perror("Unable to run Dean program");
    remove_all_semaphores();
    remove_all_shm();
    free(args.ns);

    return EXIT_FAILURE;
  }

  if (!board_runner(args.ns, args.ns_len)) {
    perror("Unable to run Board program");
    remove_all_semaphores();
    remove_all_shm();
    free(args.ns);

    return EXIT_FAILURE;
  }

  if (!students_runner(args.k, args.ns, args.t)) {
    perror("Unable to run Student program");
    remove_all_semaphores();
    remove_all_shm();
    free(args.ns);

    return EXIT_FAILURE;
  }

  free(args.ns);

  while (SIGNALED == 0) sleep(1);

  if (!remove_all_semaphores()) {
    perror("Unable to remove semaphores");

    return EXIT_FAILURE;
  }

  if (!remove_all_shm()) {
    perror("Unable to remove shared memory blocks");

    return EXIT_FAILURE;
  }
  */
  free(args.ns);
  remove_all_semaphores();
  remove_all_shared_memory();

  return EXIT_SUCCESS;
}

bool dean_runner(int k) {
  const pid_t pid = fork();

  if (pid == -1) {
    return false;
  } else if (pid == 0) {
    char* k_str = int_to_str(k);

    if (k_str == NULL) { exit(EXIT_FAILURE); }

    if (execl("./bin/dean", "dean", "-K", k_str, NULL) == -1) {
      free(k_str);
      exit(EXIT_FAILURE);
    }
  } else {
    int status;
    
    sleep(1);
    waitpid(pid, &status, WNOHANG);

    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) return false;
  }

  return true;
}

bool board_runner(int* ns, ssize_t ns_len) {
  for (size_t i = 0; i < BOARDS_LENGTH; i++) {
    const pid_t pid = fork();

    if (pid == -1) {
      return false;
    } else if (pid == 0) {
      const char b[2] = {BOARDS[i], '\0'};
      char* ns_str = int_arr_to_str(ns, ns_len);

      if (ns_str == NULL) {
        perror("Creating arguments for Board program");

        exit(EXIT_FAILURE);
      }

      if (execl("./bin/board", "board", "-B", b, "-Ns", ns_str, NULL) == -1) {
        perror("Executing Board program");
        free(ns_str);

        exit(EXIT_FAILURE);
      }
    } else {
      int status;

      sleep(1);
      waitpid(pid, &status, WNOHANG);

      if (WIFEXITED(status) && WEXITSTATUS(status) != 0) return false;
    }
  }

  return true;
}

bool students_runner(int k, int* ns, int t) {
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

        if (k_str == NULL || n_str == NULL) {
          if (k_str != NULL) free(k_str);

          if (n_str != NULL) free(n_str);

          if (t_str != NULL) free(t_str);

          exit(EXIT_FAILURE);
        }

        if (execl("./bin/student", "student", "-K", k_str, "-N", n_str, "-T",
                  t_str, NULL) == -1) {
          perror("Executing Student program");
          free(k_str);
          free(n_str);

          exit(EXIT_FAILURE);
        }
      }
    }
  }

  return true;
}

void signal_handler(int signal) {
  if (signal == SIGUSR1 && SIGNALED == 0) {
    printf("MAIN: SIGUSR1\n");

    SIGNALED = 1;
    kill(0, SIGUSR1);
  }
}

bool handle_signal() {
  struct sigaction sa;

  sa.sa_handler = signal_handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);

  if (sigaction(SIGUSR1, &sa, NULL) == -1) return false;

  return true;
}
