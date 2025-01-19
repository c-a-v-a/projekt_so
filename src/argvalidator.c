#include "argvalidator.h"

#include <stdbool.h>
#include <stddef.h>

#include "argparser.h"
#include "defaults.h"

bool validate_main_args(struct MainArgs* args) {
  bool ns_test = true;

  // Fill in
  if (args->k == -1) {
    args->k = DEFAULT_K;
  }

  if (args->t == -1) {
    args->t = DEFAULT_T;
  }

  if (args->ns_len == -1 && args->ns == NULL) {
    args->ns_len = args->k;
    args->ns = generate_random_ns(args->ns_len, MIN_N_RANGE, MAX_N_RANGE);
  }

  if (args->ns == NULL) {
    return false;
  }

  // Validation
  for (ssize_t i = 0; i < args->ns_len; i++) {
    int n = args->ns[i];
    ns_test = ns_test && n >= MIN_N_RANGE && n <= MAX_N_RANGE;
  }

  return args->k >= DEFAULT_K && args->ns_len == args->k && ns_test &&
         args->t >= 1;
}

bool validate_dean_args(struct DeanArgs* args) { return args->k >= DEFAULT_K; }

bool validate_student_args(struct StudentArgs* args) {
  return !(args->k == -1 || args->n == -1 || args->t < 1);
}

bool validate_board_args(struct BoardArgs* args) {
  bool ns_test = true;
  bool name_test = false;

  if (args->ns == NULL || args->ns_len == -1) {
    return false;
  }

  for (ssize_t i = 0; i < args->ns_len; i++) {
    int n = args->ns[i];
    ns_test = ns_test && n >= MIN_N_RANGE && n <= MAX_N_RANGE;
  }

  for (ssize_t i = 0; i < BOARDS_LEN; i++) {
    name_test = name_test || args->board_name == BOARDS[i];
  }

  return args->ns_len >= DEFAULT_K && ns_test && name_test;
}
