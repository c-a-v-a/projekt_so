#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "../argparser.h"
#include "../argprinter.h"
#include "../argvalidator.h"

int main(int argc, char** argv) {
  struct DeanArgs args = dean_args_init();
  argparse_dean(argc, argv, &args);

  if (!validate_dean_args(&args)) {
    errno = EINVAL;
    perror("Error while parsing Dean arguments");
    return EXIT_FAILURE;
  }

  print_dean_args(&args);
}
