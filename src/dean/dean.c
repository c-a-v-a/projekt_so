#include <stdio.h>

#include "../argparser.h"

int main(int argc, char** argv) {
  printf("Dean\n");
  struct DeanArgs args = argparse_dean(argc, argv);
  printf("K: %d\n", args.k);
}
