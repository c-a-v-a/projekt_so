// Module responsible for parsing command line arguments into usable format.
// It's used in both the `main` program and the subprograms.

#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <stdbool.h>
#include <sys/types.h>

// TODO: Revise documentation

// Structure that `main` program's arguments will be parsed to. `k` signifies
// amount of fields of study in our university, and `ns` is an array of amount
// of students on given field `k` (so ns[2] is amount of students in field
// `k` = 3. Value -1 for `k` and `ns_length` as well as NULL for `ns` signify,
// that those values were not passed as a argument to the program and they
// should be replaced by default constants (in case of k), or generated
// randomly (in case of ns) using `defaults` module.
struct MainArgs {
  int k;
  int* ns;
  ssize_t ns_len;
  int t;
};

// `k` is the number of all fields of study (the same as `k` in `MainArgs`).
// `k` = -1 is considered an error, so no value was passed to the program
// (theoretically unachieveable because we are spawning it from the `main`
// program.
struct DeanArgs {
  int k;
};

// `k` is the given student's fied of study and `n` is his number on the list.
// As in structures above the value -1 means, that the given parameter wasn't
// parsed i.e. wasn't passed to the program.
struct StudentArgs {
  int k;
  int n;
  int t;
};

// `ns` is an array like in `MainArgs`, while `board_name` is the name of the
// board, in our case 'A' or 'B', since we have two boards with slightly
// different roles and we need to somehow differentiate one and the other.
struct BoardArgs {
  int* ns;
  ssize_t ns_len;
  char board_name;
};

// Initializes struct with default values, so the values that represent
// that given parameter wasn't parsed.
struct MainArgs main_args_init();
struct DeanArgs dean_args_init();
struct StudentArgs student_args_init();
struct BoardArgs board_args_init();

bool argparse_main(int argc, char** argv, struct MainArgs* args);
bool argparse_dean(int argc, char** argv, struct DeanArgs* args);
bool argparse_student(int argc, char** argv, struct StudentArgs* args);
bool argparse_board(int argc, char** argv, struct BoardArgs* args);

#endif
