// Module responsible for parsing command line arguments into usable format.
// It's used in both the `main` program and the subprograms.

#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <sys/types.h>

// Structure that `main` program's arguments will be parsed to.
// Value -1 for `k` and `ns_length` as well as NULL for `ns` signify, that
// those values were not passed as a argument to the program and they should be
// replaced by default constants (in case of k), or generated randomly (in case
// of ns) using `defualts` module.
struct MainArgs {
  int k;
  int* ns;
  ssize_t ns_len;
};

struct MainArgs argparse_main(int argc, char** argv);

#endif
