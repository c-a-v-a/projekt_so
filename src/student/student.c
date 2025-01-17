#include <stdio.h>

#include "../argparser.h"

int main(int argc, char** argv) {
  struct StudentArgs args = {-1, -1};
  argparse_student(argc, argv, &args);
  printf("Student %d %d\n", args.k, args.n);
}
