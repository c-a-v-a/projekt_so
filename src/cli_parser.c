#include "cli_parser.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

struct BoardArguments initial_board() {
  return (struct BoardArguments) { NULL, -1, '\0' };
}

struct DeanArguments initial_dean() {
  return (struct DeanArguments) { NULL, -1, -1, -1 };
}

struct StudentArguments initial_student() {
  return (struct StudentArguments) { -1, -1, -1 };
}

bool parse_board(int argc, char** argv, struct BoardArguments* args) {
  if (args == NULL) {
    errno = EINVAL;
    return false;
  }

  for (int i = 1; i < argc - 1; i++) {
    const char* flag = argv[i];

    if (strcmp(flag, "-ns") == 0) {
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

      args->ns = ns;
      args->ns_len = ns_len;
    } else if (strcmp(flag, "-b") == 0) {
      char* board_string = argv[++i];

      if (strlen(board_string) != 1) {
        errno = EINVAL;
        return false;
      }

      args->board_name = board_string[0];
    } else {
      errno = EINVAL;
      return false;
    }
  }

  return true;
}

bool parse_dean(int argc, char** argv, struct DeanArguments* args) {
  if (args == NULL) {
    errno = EINVAL;
    return false;
  }

  for (int i = 1; i < argc - 1; i++) {
    const char* flag = argv[i];

    if (strcmp(flag, "-k") == 0) {
      args->k = atoi(argv[++i]);
    } else if (strcmp(flag, "-t") == 0) {
      args->t = atoi(argv[++i]);
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

      args->ns = ns;
      args->ns_len = ns_len;
    } else {
      errno = EINVAL;
      return false;
    }
  }

  return true;
}

bool parse_student(int argc, char** argv, struct StudentArguments* args) {
  if (args == NULL) {
    errno = EINVAL;
    return false;
  }

  for (int i = 1; i < argc - 1; i++) {
    const char* flag = argv[i];
    
    if (strcmp(flag, "-k") == 0) {
      args->k = atoi(argv[++i]);
    } else if (strcmp(flag, "-t") == 0) {
      args->t = atoi(argv[++i]);
    } else if (strcmp(flag, "-n") == 0) {
      args->n = atoi(argv[++i]);
    } else {
      errno = EINVAL;
      return false;
    }
  }

  return true;
}

bool fill_dean(struct DeanArguments* args) {
  if (args == NULL) {
    errno = EINVAL;
    return false;
  }

  if (args->k == -1) args->k = DEFAULT_K;

  if (args->t == -1) args->t = DEFAULT_T;

  if (args->ns_len == -1 && args->ns == NULL) {
    args->ns_len = args->k;
    args->ns = _generate_random_ns(args->ns_len, N_RANGE_MIN, N_RANGE_MAX);
  }

  return !(args->ns == NULL);
}

bool validate_board(struct BoardArguments args) {
  bool ns_test = true;
  bool name_test = false;

  for (ssize_t i = 0; i < args.ns_len; i++) {
    int n = args.ns[i];
    ns_test = ns_test && n >= N_RANGE_MIN && n <= N_RANGE_MAX;
  }

  for (ssize_t i = 0; i < BOARDS_LENGTH; i++) {
    name_test = name_test || args.board_name == BOARDS[i];
  }

  return args.ns_len >= DEFAULT_K && ns_test && name_test;
}

bool validate_dean(struct DeanArguments args) {
  bool ns_test = true;

  for (ssize_t i = 0; i < args.ns_len; i++) {
    int n = args.ns[i];
    ns_test = ns_test && n >= N_RANGE_MIN && n <= N_RANGE_MAX;
  }

  return args.k >= DEFAULT_K && args.ns_len == args.k && ns_test && 
    args.t >= 0;
}

bool validate_student(struct StudentArguments args) {
  return !(args.k == -1 || args.n == -1 || args.t < 0);
}

int* _generate_random_ns(int k, int min, int max) {
  int* ns = malloc(k * sizeof(int));

  if (ns == NULL) return ns;

  for (int i = 0; i < k; i++) {
    ns[i] = rand() % (max - min) + min;
  }

  return ns;
}

