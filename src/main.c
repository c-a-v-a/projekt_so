#include "main.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "argparser.h"
#include "defaults.h"

int main(int argc, char** argv) {
  printf("Main\n");

  srand(time(NULL));

  struct MainArgs args = argparse_main(argc, argv);

  // TODO: Add param validation
  // TODO: Add random time for process spawning

  if (args.k == -1) {
    args.k = DEFAULT_K;
  }

  if (args.ns_len == -1 && args.ns == NULL) {
    args.ns_len = args.k;
    args.ns = generate_random_ns(args.ns_len, MIN_N_RANGE, MAX_N_RANGE);
  }

  printf("PARAMS:\n");
  printf("K: %d\n", args.k);
  printf("Ns: ");
  for (int i = 0; i < args.ns_len; i++) {
    printf("%d ", args.ns[i]);
  }
  printf("\n");

  if (args.ns != NULL) {
    free(args.ns);
  }

  return 0;
}

void dean_runner(char* k) {
	const pid_t pid = fork();

	if (pid == -1) {
    perror("Creating Dean process");
    exit(1);
	} else if (pid == 0) {
		if (execl("./bin/dean", "dean", "-K", k, NULL) == -1) {
			perror("Executing Dean program");
			exit(1);
		}
	}
}

void board_runner(char* ns) {
  for (size_t i = 0; i < BOARDS_LEN; i++) {
    const char b[2] = { BOARDS[i], '\0' };

    const pid_t pid = fork();

    sleep(rand() % 5);

    if (pid == -1) {
      perror("Creating Board process");
      exit(1);
    } else if (pid == 0) {
      if (execl("./bin/board", "board", "-B", b, "-Ns", ns, NULL) == -1) {
        perror("Executing Board program");
        exit(1);
      }
    }
  }
}

void students_runner(int k, int* ns) {
  for (int i = 0; i < k; i++) {
    const int n = ns[i];

    for (int j = 1; j <= n; j++) {
      char* k = int_to_str(i + 1);
      char* n = int_to_str(j);

      // TODO: check if k or n is null

      const pid_t pid = fork();

      if (pid == -1) {
        perror("Creating Student process");
        exit(1);
      } else if (pid == 0) {
        if (execl("./bin/student", "student", "-K", k, "-N", n, NULL) == -1) {
          perror("Executing Student program");
          exit(1);
        }
      }
    }
  }
}

// TODO: Move this
// TODO: Change error handling everywhere
char* int_to_str(int x) {
  size_t str_size = (size_t) ((ceil(log10(x)) + 2) * sizeof(char));
  char* str = malloc(str_size);

  if (str == NULL) { return str; }

  if (sprintf(str, "%d", x) < 0) {
    free(str);
    return NULL;
  }

  return str;
}

char* int_arr_to_str(int* xs, size_t n) {
  if (n < 1) { return NULL; }

  size_t str_size = 0;
  char* str;

  for (size_t i = 0; i < n; i++) {
    str_size += (size_t) ((ceil(log10(xs[i])) + 2) * sizeof(char));
  }

  str = malloc(str_size);

  if (str == NULL) { return str; }

  for (size_t i = 0; i < n; i++) {
    char* s = int_to_str(xs[i]);
    strcat(str, s);
    strcat(str, ",");
    free(s);
  }

  str[str_size - 1] = '\0';

  return str;
}
