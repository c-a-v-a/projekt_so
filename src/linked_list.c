#include "linked_list.h"

#include <stdlib.h>
#include <stdio.h>

void linked_list_add(struct Node** head, int k, int n, float grade_a, float grade_b) {
  struct Node* new = malloc(sizeof(struct Node));

  if (new == NULL) return;
  
  new->k = k;
  new->n = n;
  new->grade_a = grade_a;
  new->grade_b = grade_b;
  new->next == NULL;

  if (*head == NULL) {
    *head = new;

    return;
  }

  struct Node* current = *head;
  while (current->next != NULL) {
    current = current->next;
  }

  current->next = new;
}

void linked_list_free(struct Node* head) {
  struct Node* current = head;
  struct Node* next;

  while (current != NULL && current->next != NULL) {
    next = current->next;
    free(current);
    current = next;
  }

  free(current);
}
