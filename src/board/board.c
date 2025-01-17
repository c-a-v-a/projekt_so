#include <stdio.h>

#include "../argparser.h"

int main(int argc, char** argv) {
  struct BoardArgs args = argparse_board(argc, argv);
  printf("B:%c Ns:", args.board_name);
  for (int i = 0; i < args.ns_len; i++) printf("%d ", args.ns[i]);
  printf("\n");
}
