#include "cli_parser.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

struct BoardArguments initial_board() { return (struct BoardArguments){'\0'}; }

struct DeanArguments initial_dean() { return (struct DeanArguments){-1}; }

struct MainArguments initial_main() {
  return (struct MainArguments){NULL, -1, -1, -1};
}

struct StudentArguments initial_student() {
  return (struct StudentArguments){-1, -1, -1};
}

bool parse_board(int argc, char** argv, struct BoardArguments* arguments) {
  if (arguments == NULL) {
    errno = EINVAL;
    return false;
  }

  for (int i = 1; i < argc - 1; i++) {
    const char* flag = argv[i];

    if (strcmp(flag, "-b") == 0) {
      char* board_string = argv[++i];

      if (strlen(board_string) != 1) {
        errno = EINVAL;
        return false;
      }

      arguments->board_name = board_string[0];
    } else {
      errno = EINVAL;
      return false;
    }
  }

  return true;
}

bool parse_dean(int argc, char** argv, struct DeanArguments* arguments) {
  if (arguments == NULL) {
    errno = EINVAL;
    return false;
  }

  for (int i = 1; i < argc - 1; i++) {
    const char* flag = argv[i];

    if (strcmp(flag, "-k") == 0) {
      arguments->k = atoi(argv[++i]);
    } else {
      errno = EINVAL;
      return false;
    }
  }

  return true;
}

bool parse_main(int argc, char** argv, struct MainArguments* arguments) {
  if (arguments == NULL) {
    errno = EINVAL;
    return false;
  }

  for (int i = 1; i < argc - 1; i++) {
    const char* flag = argv[i];

    if (strcmp(flag, "-k") == 0) {
      arguments->k = atoi(argv[++i]);
    } else if (strcmp(flag, "-t") == 0) {
      arguments->t = atoi(argv[++i]);
    } else if (strcmp(flag, "-ns") == 0) {
      char* ns_string = argv[++i];
      int* ns = NULL;
      ssize_t ns_len = 0;
      char* token = strtok(ns_string, ",");

      do {
        ns_len++;
        ns = realloc(ns, ns_len * sizeof(int));

        if (ns == NULL) return false;

        ns[ns_len - 1] = atoi(token);
        token = strtok(NULL, ",");
      } while (token != NULL);

      arguments->ns = ns;
      arguments->ns_len = ns_len;
    } else {
      errno = EINVAL;
      return false;
    }
  }

  return true;
}

bool parse_student(int argc, char** argv, struct StudentArguments* arguments) {
  if (arguments == NULL) {
    errno = EINVAL;
    return false;
  }

  for (int i = 1; i < argc - 1; i++) {
    const char* flag = argv[i];

    if (strcmp(flag, "-k") == 0) {
      arguments->k = atoi(argv[++i]);
    } else if (strcmp(flag, "-t") == 0) {
      arguments->t = atoi(argv[++i]);
    } else if (strcmp(flag, "-n") == 0) {
      arguments->n = atoi(argv[++i]);
    } else {
      errno = EINVAL;
      return false;
    }
  }

  return true;
}

bool fill_main(struct MainArguments* arguments) {
  if (arguments == NULL) {
    errno = EINVAL;
    return false;
  }

  if (arguments->k == -1) arguments->k = DEFAULT_K;

  if (arguments->t == -1) arguments->t = DEFAULT_T;

  if (arguments->ns_len == -1 && arguments->ns == NULL) {
    arguments->ns_len = arguments->k;
    arguments->ns =
        _generate_random_ns(arguments->ns_len, N_RANGE_MIN, N_RANGE_MAX);
  }

  return !(arguments->ns == NULL);
}

bool validate_board(struct BoardArguments arguments) {
  bool name_test = false;

  for (ssize_t i = 0; i < BOARDS_LENGTH; i++) {
    name_test = name_test || arguments.board_name == BOARDS[i];
  }

  return name_test;
}

bool validate_dean(struct DeanArguments arguments) { return arguments.k > -1; }

bool validate_main(struct MainArguments arguments) {
  bool ns_test = true;

  for (ssize_t i = 0; i < arguments.ns_len; i++) {
    int n = arguments.ns[i];
    ns_test = ns_test && n >= N_RANGE_MIN && n <= N_RANGE_MAX;
  }

  return arguments.k >= DEFAULT_K && arguments.ns_len == arguments.k &&
         ns_test && arguments.t >= 0;
}

bool validate_student(struct StudentArguments arguments) {
  return !(arguments.k == -1 || arguments.n == -1 || arguments.t < 0);
}

int* _generate_random_ns(int k, int min, int max) {
  int* ns = malloc(k * sizeof(int));

  if (ns == NULL) return ns;

  for (int i = 0; i < k; i++) {
    ns[i] = rand() % (max - min) + min;
  }

  return ns;
}
