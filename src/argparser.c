#include "argparser.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

struct MainArgs argparse_main(int argc, char** argv) {
  struct MainArgs parsed = { -1, NULL, -1 };

  for (int i = 1; i < argc - 1; i++) {
    const char* flag = argv[i];
    
    if (strcmp(flag, "-K") == 0) {
      parsed.k = atoi(argv[++i]);
    } else if (strcmp(flag, "-Ns") == 0) {
      char* ns_string = argv[++i];
      int* ns = NULL;
      ssize_t ns_len = 0;
      char* token = strtok(ns_string, ",");

      do {
        ns_len++;
        ns = realloc(ns, ns_len * sizeof(int));

        if (ns == NULL) {
          errno = ENOMEM;
          perror("Parsing Main progam arguments");
          exit(1);
        }

        ns[ns_len - 1] = atoi(token);
        token = strtok(NULL, ",");
      } while (token != NULL);

      parsed.ns = ns;
      parsed.ns_len = ns_len;
    } else {
      errno = EINVAL;
      perror("Parsing Main progam arguments");
      exit(1);
    }
  }

  return parsed;
}

struct DeanArgs argparse_dean(int argc, char** argv) {
  struct DeanArgs parsed = { -1 };

  for (int i = 1; i < argc; i++) {
    const char* flag = argv[i];
    if (strcmp(flag, "-K") == 0) {
      parsed.k = atoi(argv[++i]);
    } else {
      errno = EINVAL;
      perror("Parsing Dean progam arguments");
      exit(1);
    }
  }

  return parsed;
}

struct StudentArgs argparse_student(int argc, char** argv) {
  struct StudentArgs parsed = { -1, -1 };

  for (int i = 1; i < argc; i++) {
    const char* flag = argv[i];
    if (strcmp(flag, "-K") == 0) {
      parsed.k = atoi(argv[++i]);
    } else if (strcmp(flag, "-N") == 0) {
      parsed.n = atoi(argv[++i]);
    } else {
      errno = EINVAL;
      perror("Parsing Student progam arguments");
      exit(1);
    }
  }

  return parsed;
}

struct BoardArgs argparse_board(int argc, char** argv) {
  struct BoardArgs parsed = { NULL, -1, '\0' };

  for (int i = 1; i < argc - 1; i++) {
    const char* flag = argv[i];
    
    if (strcmp(flag, "-Ns") == 0) {
      char* ns_string = argv[++i];
      int* ns = NULL;
      ssize_t ns_len = 0;
      char* token = strtok(ns_string, ",");

      do {
        ns_len++;
        ns = realloc(ns, ns_len * sizeof(int));

        if (ns == NULL) {
          errno = ENOMEM;
          perror("Parsing Main progam arguments");
          exit(1);
        }

        ns[ns_len - 1] = atoi(token);
        token = strtok(NULL, ",");
      } while (token != NULL);

      parsed.ns = ns;
      parsed.ns_len = ns_len;
    } else if (strcmp(flag, "-B") == 0) {
      char* board_string = argv[++i];

      if (strlen(board_string) == 1) {
        parsed.board_name = board_string[0];
      } else {
        errno = EINVAL;
        perror("Parsing Main progam arguments");
        exit(1);
      }
    } else {
      errno = EINVAL;
      perror("Parsing Main progam arguments");
      exit(1);
    }
  }

  return parsed;
}
