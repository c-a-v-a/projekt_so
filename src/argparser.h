// Module responsible for parsing command line arguments into usable format.
// It's used in both the `main` program and the subprograms.

#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <sys/types.h>

// Structure that `main` program's arguments will be parsed to. All of the
// fields of the struct are optional (they may be NULL). If the fields happen
// to be NULL, then the `defaults` module should be used to fill in the empty
// field.
struct MainArgs {
  int k;
  int* ns;
  ssize_t ns_len;
};

struct MainArgs argparse_main(int argc, char** argv);

#endif
