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

#include "argparser.h"
#include "argprinter.h"
#include "argvalidator.h"
#include "defaults.h"
#include "str_creator.h"

volatile sig_atomic_t SIGNALED = 0;

int main(int argc, char** argv) {
  struct MainArgs args = main_args_init();

  srand(time(NULL));

  if (setpgid(0, 0) == -1) {
    perror("Unable to create a pid group");
    return EXIT_FAILURE;
  }

  if (!handle_signal()) {
    perror("Unable to register signal handler");
    return EXIT_FAILURE;
  }

  if (!argparse_main(argc, argv, &args)) {
    perror("Unable to parse Main arguments");
    return EXIT_FAILURE;
  }

  if (!validate_main_args(&args)) {
    if (args.ns != NULL) free(args.ns);

    errno = EINVAL;
    perror("Error while parsing Main arguments");

    return EXIT_FAILURE;
  }

  print_main_args(&args);

  if (!dean_runner(args.k)) {
    perror("Unable to run Dean program");
    free(args.ns);

    return EXIT_FAILURE;
  }

  if (!board_runner(args.ns, args.ns_len)) {
    perror("Unable to run Board program");
    free(args.ns);

    return EXIT_FAILURE;
  }

  if (!students_runner(args.k, args.ns)) {
    perror("Unable to run Student program");
    free(args.ns);

    return EXIT_FAILURE;
  }

  free(args.ns);

  while (true) sleep(10);

  return EXIT_SUCCESS;
}

bool dean_runner(int k) {
  const pid_t pid = fork();

  if (pid == -1) {
    return false;
  } else if (pid == 0) {
    char* k_str = int_to_str(k);

    if (k_str == NULL) {
      perror("Creating arguments for Dean program");

      exit(EXIT_FAILURE);
    }

    if (execl("./bin/dean", "dean", "-K", k_str, NULL) == -1) {
      perror("Executing Dean program");
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
  for (size_t i = 0; i < BOARDS_LEN; i++) {
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

bool students_runner(int k, int* ns) {
  for (int i = 0; i < k; i++) {
    const int n = ns[i];

    for (int j = 1; j <= n; j++) {
      const pid_t pid = fork();

      if (pid == -1) {
        return false;
      } else if (pid == 0) {
        char* k_str = int_to_str(i + 1);
        char* n_str = int_to_str(j);

        if (k_str == NULL || n_str == NULL) {
          if (k_str != NULL) free(k_str);

          if (n_str != NULL) free(n_str);

          exit(EXIT_FAILURE);
        }

        if (execl("./bin/student", "student", "-K", k_str, "-N", n_str, NULL) ==
            -1) {
          perror("Executing Student program");
          free(k_str);
          free(n_str);

          exit(EXIT_FAILURE);
        }
      } else {
        int status;

        sleep(1);
        waitpid(pid, &status, WNOHANG);

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) return false;
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
