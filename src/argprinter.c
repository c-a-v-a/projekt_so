#include "argprinter.h"

#include <stdio.h>
#include <sys/types.h>

#include "cli_parser.h"

void print_dean_args(struct DeanArguments* args) {
  printf("DEAN\t");
  printf("K:%d\n", args->k);
}

void print_student_args(struct StudentArguments* args) {
  printf("STUDENT\t");
  printf("K:%d N:%d T:%d\n", args->k, args->n, args->t);
}

void print_board_args(struct BoardArguments* args) {
  printf("BOARD\t");
  printf("NAME:%c Ns: ", args->board_name);

  for (ssize_t i = 0; i < args->ns_len; i++) printf("%d ", args->ns[i]);

  printf("\n");
}
