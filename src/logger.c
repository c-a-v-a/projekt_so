#include "logger.h"

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>

bool logger(char* format, ...) {
  FILE* log_file = fopen(LOG_FILE_NAME, "a");

  if (log_file == NULL) return false;

  va_list arguments;
  bool result = true;

  va_start(arguments, format);
  result = result && vprintf(format, arguments) >= 0;
  va_end(arguments);

  va_start(arguments, format);
  result = result && vfprintf(log_file, format, arguments) >= 0;
  va_end(arguments);

  fclose(log_file);

  return result;
}

bool log_board_spawned(struct BoardArguments arguments) {
  bool result = true;

  result = result && logger("Spawned board %c NS:", arguments.board_name);

  /*
  for (ssize_t i = 0; i < arguments.ns_len; i++) {
    result = result && logger("%d ", arguments.ns[i]);
  }
  */

  result = result && logger("\n");

  return result;
}

bool log_dean_spawned(struct DeanArguments arguments) {
  return logger("Spawned dean\tK:%d\n", arguments.k);
}

bool log_main_spawned(struct MainArguments arguments) {
  bool result = true;

  result =
      result && logger("Spawned main\tK:%d T:%d NS:", arguments.k, arguments.t);

  for (ssize_t i = 0; i < arguments.ns_len; i++) {
    result = result && logger("%d ", arguments.ns[i]);
  }

  result = result && logger("\n");

  return result;
}

bool log_student_spawned(struct StudentArguments arguments) {
  return logger("Spawned student\tK:%d N:%d T:%d\n", arguments.k, arguments.n,
                arguments.t);
}
