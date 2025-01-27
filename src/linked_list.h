#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct Node {
  int k;
  int n;
  float grade_a;
  float grade_b;
  struct Node* next;
};

void linked_list_add(struct Node** head, int k, int n, float grade_a, float grade_b);
void linked_list_free(struct Node* head);

#endif
