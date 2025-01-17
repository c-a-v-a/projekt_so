#include <stdio.h>

#include "../argparser.h"
#include "../argprinter.h"

int main(int argc, char** argv) {
  struct BoardArgs args = board_args_init();
  argparse_board(argc, argv, &args);
  print_board_args(&args);
}
