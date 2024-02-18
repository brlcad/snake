#include <stdlib.h>
#include <stdio.h>

#include "snake.h"

Node *newNode(const Point pos, Node *prev) {
  Node *self = malloc(sizeof(Node));

  self->pos = pos;
  self->prev = prev;
  self->next = NULL;

  return self;
}

void destroyNode(Node *self) {
  if (self != NULL)
    free(self);
}

Snake *newSnake(const Point center) {
  Snake *self = malloc(sizeof(Snake));
  self->tail = self->head = newNode(center, NULL);
  self->direction = rand() % (WEST + 1);
  self->length = 1;
  return self;
}

void destroySnake(Snake *self) {
  if (self != NULL) {
    Node *current = self->head, *next;
    while (current != NULL) {
      next = current->next;
      destroyNode(current);
      current = next;
    }
    free(self);
  }
}

bool selfCollision(const Snake *self, Point *collision) {
  for (Node *it1 = self->head; it1 != NULL; it1 = it1->prev)
    for (Node *it2 = it1->prev; it2 != NULL; it2 = it2->prev)
      if (it1->pos.x == it2->pos.x && it1->pos.y == it2->pos.y) {
        *collision = it1->pos;
        return true;
      }
  return false;
}

Node *advance(Snake *self) {
  // Copy the position of the current head
  Point newHeadPosition = self->head->pos;

  // Move it forward in the current direction
  switch (self->direction) {
  case NORTH:
    --newHeadPosition.y;
    break;
  case EAST:
    ++newHeadPosition.x;
    break;
  case SOUTH:
    ++newHeadPosition.y;
    break;
  case WEST:
    --newHeadPosition.x;
    break;
  }

  // Push new head
  self->head->next = newNode(newHeadPosition, self->head);
  self->head = self->head->next;

  // Pop the tail
  Node *oldTail = self->tail;
  self->tail = self->tail->next;
  self->tail->prev = NULL;

  return oldTail;
}

void changeDirection(Snake *self, Direction direction) {
  // Disallow moving from SOUTH to NORTH and vice versa and from EAST to WEST
  // and vice versa when the Snake is longer than 1 Point
  if (direction == self->direction ||
      (self->length > 1 && direction == (self->direction + 2) % (WEST + 1)))
    return;

  self->direction = direction;
}

void grow(Snake *self, Node *oldTail) {
  oldTail->next = self->tail;
  self->tail = oldTail;

  ++self->length;
}