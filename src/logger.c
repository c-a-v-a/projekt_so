#include "logger.h"

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>

FILE* log_file = NULL;

bool open_log_file() {
  if (log_file != NULL) return false;

  log_file = fopen(LOG_FILE_NAME, "w");

  return !(log_file == NULL);
}

bool close_log_file() {
  if (log_file == NULL) return false;

  return fclose(log_file) == 0;
}

bool logger(char* format, ...) {
  if (log_file == NULL) return false;

  va_list arguments;
  bool result = true;

  va_start(arguments, format);
  result = result && vprintf(format, arguments) >= 0;
  va_end(arguments);

  va_start(arguments, format);
  result = result && vfprintf(log_file, format, arguments) >= 0;
  va_end(arguments);

  return result;
}

bool log_board_spawned(struct BoardArguments arguments) {
  bool result = true;

  result = result && logger("SPAWNED BOARD %c NS:", arguments.board_name);

  for (ssize_t i = 0; i < arguments.ns_len; i++) {
    result = result && logger("%d ", arguments.ns[i]);
  }

  result = result && logger("\n");

  return result;
}

bool log_dean_spawned(struct DeanArguments arguments) {
  return logger("Spawned dean\tK:%d\n", arguments.k);
}

bool log_main_spawned(struct MainArguments arguments) {
  bool result = true;

  result = result && logger("SPAWNED DEAN\tK:%d T:%d NS:", arguments.k, arguments.t);

  for (ssize_t i = 0; i < arguments.ns_len; i++) {
    result = result && logger("%d ", arguments.ns[i]);
  }

  result = result && logger("\n");

  return result;
}

bool log_student_spawned(struct StudentArguments arguments) {
  return logger("SPAWNED STUDENT\tK:%d T:%d N:%d\n", arguments.k, arguments.t, arguments.n);
}
