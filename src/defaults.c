#include "defaults.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int* generate_random_ns(int k, int min, int max) {
  int* ns = malloc(k * sizeof(int));

  if (ns == NULL) {
    errno = ENOMEM;
    perror("Generating Ns values");
    exit(1);
  }
  
  for (int i = 0; i < k; i++) {
    ns[i] = rand() % (max - min) + min;
  }

  return ns;
}
