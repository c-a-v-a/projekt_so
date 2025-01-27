#include "grades.h"

#include <stdlib.h>

float get_random_grade() {
  float grade = -1;
  int probability = rand() % 100 + 1;
  int i = 0;

  for (; i < GRADES_LEN; i++) {
    probability -= GRADES_PROBABILITY[i];
    grade = GRADES[i];

    if (probability <= 0) break;
  }
  
  return grade;
}

float get_average(float x, float y, float z) {
  float avg = (x + y + z) / 3;

  for (int i = 0; i < GRADES_LEN; i++) {
    if (GRADES[i] >= avg) return GRADES[i];
  }

  return -1;
}

float get_final_grade(float x, float y) {
  if (x < 3 || y < 3) return 2.0;

  float avg = (x + y) / 2;

  for (int i = 0; i < GRADES_LEN; i++) {
    if (GRADES[i] >= avg) return GRADES[i];
  }

  return -1;
}
