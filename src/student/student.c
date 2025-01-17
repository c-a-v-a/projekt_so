#include <stdio.h>

#include "../argparser.h"
#include "../argprinter.h"

int main(int argc, char** argv) {
  struct StudentArgs args = student_args_init();
  argparse_student(argc, argv, &args);
  print_student_args(&args);
}
