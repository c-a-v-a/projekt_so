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
  srand(time(NULL));
  setpgid(0, 0); // Handle error

  struct sigaction sa;
  sa.sa_handler = signal_handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  if (sigaction(SIGUSR1, &sa, NULL) == -1) {
    perror("Unable to register SIGINT handler");
    exit(1);
  }

  struct MainArgs args = main_args_init();
  
  if (!argparse_main(argc, argv, &args)) {
    return EXIT_FAILURE;
  }

  if (!validate_main_args(&args)) {
    if (args.ns != NULL) { free(args.ns); }
    errno = EINVAL;
    perror("Error while parsing Main arguments");
    return EXIT_FAILURE;
  }

  print_main_args(&args);

  if(!dean_runner(args.k)) {
    free(args.ns);
    return EXIT_FAILURE;
  }
  
  if(!board_runner(args.ns, args.ns_len)) {
    free(args.ns);
    return EXIT_FAILURE;
  }

  if(!students_runner(args.k, args.ns)) {
    free(args.ns);
    return EXIT_FAILURE;
  }

  free(args.ns);

  while (true) {
    sleep(10);
  }

  return EXIT_SUCCESS;
}

bool dean_runner(int k) {
  const pid_t pid = fork();

  if (pid == -1) {
    perror("Creating Dean process");
    return false;
  } else if (pid == 0) {
    char* k_str = int_to_str(k);
    
    if (k_str == NULL) {
      perror("Creating arguments for Dean program");
      exit(EXIT_FAILURE);
    }

    if (execl("./bin/dean", "dean", "-K", k_str, NULL) == -1) {
      free(k_str);
      perror("Executing Dean program");
      exit(EXIT_FAILURE);
    }
  } else {
    sleep(1);

    int status;
    pid_t result = waitpid(pid, &status, WNOHANG);

    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
      return false;
    }
  }

  return true;
}

bool board_runner(int* ns, ssize_t ns_len) {
  for (size_t i = 0; i < BOARDS_LEN; i++) {
    const pid_t pid = fork();

    if (pid == -1) {
      perror("Creating Board process");
      return false;
    } else if (pid == 0) {
      const char b[2] = { BOARDS[i], '\0' };
      char* ns_str = int_arr_to_str(ns, ns_len);

      if (ns_str == NULL) {
        perror("Creating arguments for Board program");
        exit(EXIT_FAILURE);
      }

      if (execl("./bin/board", "board", "-B", b, "-Ns", ns_str, NULL) == -1) {
        free(ns_str);
        perror("Executing Board program");
        exit(EXIT_FAILURE);
      }
    } else {
      sleep(1);

      int status;
      pid_t result = waitpid(pid, &status, WNOHANG);

      if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
        return false;
      }
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
        perror("Creating Student process");
        return false;
      } else if (pid == 0) {
        char* k_str = int_to_str(i + 1);
        char* n_str = int_to_str(j);

        if (k_str == NULL || n_str == NULL) {
          if (k_str != NULL) { free(k_str); }
          if (n_str != NULL) { free(n_str); }

          exit(EXIT_FAILURE);
        }

        if (execl("./bin/student", "student", "-K", k_str, "-N", n_str, NULL) == -1) {
          free(k_str);
          free(n_str);
          perror("Executing Student program");
          exit(EXIT_FAILURE);
        }
      } else {
        sleep(1);

        int status;
        pid_t result = waitpid(pid, &status, WNOHANG);

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
          return false;
        }
      }
    }
  }

  return true;
}

// TODO: Change sleep to milliseconds sleep
void signal_handler(int signal) {
  if (signal == SIGUSR1 && SIGNALED == 0) {
    SIGNALED = 1;
    printf("MAIN: SIGUSR1\n");
    kill(0, SIGUSR1);
  }
}
