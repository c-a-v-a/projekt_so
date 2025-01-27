#include "logger.h"

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>

bool logger(const char* prefix, char* format, ...) {
  va_list arguments;
  bool result = true;

  va_start(arguments, format);
  result = result && printf("%s", prefix);
  result = result && vprintf(format, arguments) >= 0;
  result = result && printf("%s", SUFFIX);
  fflush(stdout);
  va_end(arguments);

  return result;
}

bool log_board_spawned(struct BoardArguments arguments) {
  bool result = true;

  result = result &&
           logger(BOARD_PREFIX, "Spawned board %c\n", arguments.board_name);

  return result;
}

bool log_dean_spawned(struct DeanArguments arguments) {
  return logger(DEAN_PREFIX, "Spawned dean\tK:%d\n", arguments.k);
}

bool log_main_spawned(struct MainArguments arguments) {
  bool result = true;

  result = logger(MAIN_PREFIX, "Spawned main\tK:%d T:%d NS:", arguments.k,
    arguments.t);

  for (ssize_t i = 0; i < arguments.ns_len; i++) {
    result = result && logger(MAIN_PREFIX, "%d ", arguments.ns[i]);
  }

  result = result && printf("\n");

  return result;
}

bool log_student_spawned(struct StudentArguments arguments) {
  return logger(STUDENT_PREFIX, "Spawned student[%d,%d]\n",
                arguments.k, arguments.n);
}
