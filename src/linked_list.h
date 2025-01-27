/**
 * @file linked_list.h
 * @author Filip Cebula
 * @date 27 Jan 2025
 * @brief Module that provides basic linked list functionality.
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/**
 * @brief Structure representing a node in a linked list.
 */
struct Node {
  int k;             /**< Student's major. */
  int n;             /**< Student's number. */
  float grade_a;     /**< Grade from board a. */
  float grade_b;     /**< Grade from board b. */
  struct Node* next; /**< Pointer to the next element on the list. */
};

/**
 * @brief Adds element to linked list. I can fail when malloc fails.
 *
 * @param head First element of the list.
 * @param k Student's major.
 * @param n Student's number.
 * @param grade_a Grade from board a.
 * @param grade_b Grade from board b.
 */
void linked_list_add(struct Node** head, int k, int n, float grade_a,
                     float grade_b);

/**
 * @brief Frees the liked list structure.
 *
 * @param head First element of the list.
 */
void linked_list_free(struct Node* head);

#endif
