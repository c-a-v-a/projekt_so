#include "linked_list.h"

#include <stdlib.h>

void add(struct Node* head, int k, int n, float grade_a, float grade_b) {
  struct Node* new = malloc(sizeof(struct Node));
  struct Node* current = head;

  if (new == NULL) return void;
  
  new.k = k;
  new.n = n;
  new.grade_a = grade_a;
  new.grade_b = grade_b;

  if (head == NULL) {
    head = new;

    return void;
  }

  while (current->next != NULL) {
    current = current->next;
  }

  current->next = new;
}
