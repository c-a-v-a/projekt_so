#include <stdio.h>

#include "../argparser.h"

int main(int argc, char** argv) {
  printf("Dean\n");
  struct DeanArgs args = {-1};
  argparse_dean(argc, argv, &args);
  printf("K: %d\n", args.k);
}
