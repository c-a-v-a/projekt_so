#include <stdio.h>

#include "../argparser.h"
#include "../argprinter.h"

int main(int argc, char** argv) {
  struct DeanArgs args = dean_args_init();
  argparse_dean(argc, argv, &args);
  print_dean_args(&args);
}
