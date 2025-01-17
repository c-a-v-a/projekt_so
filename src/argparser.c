#include "argparser.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

bool argparse_main(int argc, char** argv, struct MainArgs* args) {
  for (int i = 1; i < argc - 1; i++) {
    const char* flag = argv[i];
    
    if (strcmp(flag, "-K") == 0) {
      args->k = atoi(argv[++i]);
    } else if (strcmp(flag, "-Ns") == 0) {
      char* ns_string = argv[++i];
      int* ns = NULL;
      ssize_t ns_len = 0;
      char* token = strtok(ns_string, ",");

      do {
        ns_len++;
        ns = realloc(ns, ns_len * sizeof(int));

        if (ns == NULL) {
          perror("Parsing Main progam arguments");
          return false;
        }

        ns[ns_len - 1] = atoi(token);
        token = strtok(NULL, ",");
      } while (token != NULL);

      args->ns = ns;
      args->ns_len = ns_len;
    } else {
      errno = EINVAL;
      perror("Parsing Main progam arguments");
      return false;
    }
  }

  return true;
}

bool argparse_dean(int argc, char** argv, struct DeanArgs* args) {
  for (int i = 1; i < argc; i++) {
    const char* flag = argv[i];

    if (strcmp(flag, "-K") == 0) {
      args->k = atoi(argv[++i]);
    } else {
      errno = EINVAL;
      perror("Parsing Dean progam arguments");
      return false;
    }
  }

  return true;
}

bool argparse_student(int argc, char** argv, struct StudentArgs* args) {
  for (int i = 1; i < argc; i++) {
    const char* flag = argv[i];

    if (strcmp(flag, "-K") == 0) {
      args->k = atoi(argv[++i]);
    } else if (strcmp(flag, "-N") == 0) {
      args->n = atoi(argv[++i]);
    } else {
      errno = EINVAL;
      perror("Parsing Student progam arguments");
      return false;
    }
  }

  return true;
}

bool argparse_board(int argc, char** argv, struct BoardArgs* args) {
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
          perror("Parsing Board progam arguments");
          return false;
        }

        ns[ns_len - 1] = atoi(token);
        token = strtok(NULL, ",");
      } while (token != NULL);

      args->ns = ns;
      args->ns_len = ns_len;
    } else if (strcmp(flag, "-B") == 0) {
      char* board_string = argv[++i];

      if (strlen(board_string) == 1) {
        args->board_name = board_string[0];
      } else {
        errno = EINVAL;
        perror("Parsing Board progam arguments");
        return false;
      }
    } else {
      errno = EINVAL;
      perror("Parsing Board progam arguments");
      return false;
    }
  }

  return true;
}
