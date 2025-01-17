#include "argprinter.h"

#include <stdio.h>
#include <sys/types.h>

#include "argparser.h"

void print_main_args(struct MainArgs* args) {
  printf("MAIN\t");
  printf("K:%d Ns: ", args->k);
  for (ssize_t i = 0; i < args->ns_len; i++)
    printf("%d ", args->ns[i]);
  printf("\n");
}

void print_dean_args(struct DeanArgs* args) {
  printf("DEAN\t");
  printf("K:%d\n", args->k);
}

void print_student_args(struct StudentArgs* args) {
  printf("STUDENT\t");
  printf("K:%d N:%d\n", args->k, args->n);
}

void print_board_args(struct BoardArgs* args) {
  printf("BOARD\t");
  printf("NAME:%c Ns: ", args->board_name);
  for (ssize_t i = 0; i < args->ns_len; i++)
    printf("%d ", args->ns[i]);
  printf("\n");
}
