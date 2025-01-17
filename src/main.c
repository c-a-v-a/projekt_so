#include "main.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "argparser.h"
#include "argprinter.h"
#include "defaults.h"
#include "str_creator.h"

int main(int argc, char** argv) {
  srand(time(NULL));

  struct MainArgs args = main_args_init();
  
  if (!argparse_main(argc, argv, &args)) {
    return EXIT_FAILURE;
  }

  if (args.k == -1) {
    args.k = DEFAULT_K;
  }

  if (args.ns_len == -1 && args.ns == NULL) {
    args.ns_len = args.k;
    args.ns = generate_random_ns(args.ns_len, MIN_N_RANGE, MAX_N_RANGE);
  }
  // TODO: Add param validation

  if (args.ns == NULL) { return EXIT_FAILURE; }

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

  // TODO: Add ctr c handling
  while (true) { sleep(10); }

  return EXIT_SUCCESS;
}

// TODO: Add random time for process spawning
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
      }
    }
  }

  return true;
}

// TODO: Add signal for detecting when execl or building arguments failed
