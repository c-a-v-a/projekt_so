#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "../argparser.h"
#include "../argprinter.h"
#include "../argvalidator.h"

int main(int argc, char** argv) {
  struct StudentArgs args = student_args_init();
  argparse_student(argc, argv, &args);

  if (!validate_student_args(&args)) {
    errno = EINVAL;
    perror("Error while parsing Student arguments");
    return EXIT_FAILURE;
  }

  print_student_args(&args);
}
