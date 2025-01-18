#include "defaults.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int* generate_random_ns(int k, int min, int max) {
  int* ns = malloc(k * sizeof(int));

  if (ns == NULL) return ns;

  for (int i = 0; i < k; i++) ns[i] = rand() % (max - min) + min;

  return ns;
}
