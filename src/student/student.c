#include <stdio.h>

#include "../argparser.h"

int main(int argc, char** argv) {
  struct StudentArgs args = argparse_student(argc, argv);
  printf("Student %d %d\n", args.k, args.n);
}
