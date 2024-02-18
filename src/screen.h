// vi:ft=c

#ifndef SCREEN_H
#define SCREEN_H

#include <stdbool.h>
#include <stdlib.h>

#include "snake.h"

typedef enum { INCREMENTAL, EASY, MEDIUM, HARD } Difficulty;

typedef struct screen {
  int screenWidth, screenHeight;
  int width, height; // The playing field occupies a fraction of the screen

  // An offset from the top-left corner to apply to all coordinates
  // in order to center the playing field
  Point offset;

  bool *grid; // Keep track of the occupied cells. Useful when generating orbs.
  // grid will represent the playing field which is a 2D surface, but the
  // constructor allocates a 1D array of size width * height.
  // To subscript such an array i * width + j is used.

  Point orb; // The orb to capture

  Difficulty difficulty;
} Screen;

Screen *newScreen(void);
void destroyScreen(Screen *self);

void initializeNcurses(void);

void drawWalls(const Screen *self);

void updateScore(const Screen *self, const unsigned score);

// Welcome screen
// Returns true if the player choose to play, false if they choose to quit
bool welcome(Screen *self);

// Spawn a new orb
void spawnOrb(Screen *self);

// Check for collisions
bool insideBoundaries(const Screen *self, const Snake *snake);

// Draw the Snake on to the Screen
void draw(const Screen *self, const Snake *snake, bool growing,
          const Node *oldTail);

bool gameOver(Screen *self, Snake *snake, Point *collision, float *progress);

#endif // !SCREEN_H