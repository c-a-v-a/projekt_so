#include "str_creator.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

char* int_to_str(int x) {
  size_t str_size = (size_t)((ceil(log10(x)) + 2) * sizeof(char));
  char* str = malloc(str_size);

  if (str == NULL) return str;

  if (sprintf(str, "%d", x) < 0) {
    // Should never happen
    free(str);
    return NULL;
  }

  return str;
}

char* int_arr_to_str(int* xs, ssize_t n) {
  size_t str_size = 0;
  char* str;

  if (n < 1) return NULL;

  for (ssize_t i = 0; i < n; i++)
    str_size += (size_t)((ceil(log10(xs[i])) + 2) * sizeof(char));

  str = malloc(str_size);

  if (str == NULL) return str;

  for (ssize_t i = 0; i < n; i++) {
    char* s = int_to_str(xs[i]);
    strcat(str, s);
    strcat(str, ",");
    free(s);
  }

  str[str_size - 1] = '\0';

  return str;
}
