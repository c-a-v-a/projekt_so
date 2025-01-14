#include "main.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "argparser.h"

int main(int argc, char** argv) {
  printf("Main\n");

  struct MainArgs args = argparse_main(argc, argv);

  if (args.ns != NULL) {
    free(args.ns);
  }

  return 0;
}

void dean_runner() {
	const pid_t pid = fork();

	if (pid == -1) {
    perror("Creating Dean process");
    exit(1);
	} else if (pid == 0) {
		if (execl("./bin/dean", "dean", NULL) == -1) {
			perror("Executing Dean program");
			exit(1);
		}
	}
}

void board_runner() {
  // TODO: Add passing args to runner
  // TODO: Move this to global config headers
  const char boards[] = { 'A', 'B' };
  const int boards_length = 2;

  for (int i = 0; i < boards_length; i++) {
    const pid_t pid = fork();

    if (pid == -1) {
      perror("Creating Board process");
      exit(1);
    } else if (pid == 0) {
      if (execl("./bin/board", "board", NULL) == -1) {
        perror("Executing Board program");
        exit(1);
      }
    }
  }
}

void students_runner() {
  // TODO: Add passing args to runner
  // TODO: Random or argv K and N
  const int K = 2;
  const int Ns[] = { 5, 3 };

  for (int i = 0; i < K; i++) {
    const int N = Ns[i];

    for (int j = 0; j < N; j++) {
      const pid_t pid = fork();

      if (pid == -1) {
        perror("Creating Student process");
        exit(1);
      } else if (pid == 0) {
        if (execl("./bin/student", "student", NULL) == -1) {
          perror("Executing Student program");
          exit(1);
        }
      }
    }
  }
}
