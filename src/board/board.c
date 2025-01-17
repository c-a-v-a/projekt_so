#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "../argparser.h"
#include "../argprinter.h"
#include "../argvalidator.h"

int main(int argc, char** argv) {
  struct BoardArgs args = board_args_init();
  argparse_board(argc, argv, &args);

  if (!validate_board_args(&args)) {
    if (args.ns != NULL) {
      free(args.ns);
    }
    errno = EINVAL;
    perror("Error while parsing Board arguments");
    return EXIT_FAILURE;
  }

  print_board_args(&args);
}
