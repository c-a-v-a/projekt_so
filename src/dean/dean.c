#include <stdio.h>

#include "../argparser.h"

int main(int argc, char** argv) {
  printf("Dean\n");
  struct DeanArgs args = dean_args_init();
  argparse_dean(argc, argv, &args);
  printf("K: %d\n", args.k);
}
