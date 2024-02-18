#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "screen.h"
#include "snake.h"

int main(void) {
  setlocale(LC_ALL, ""); // Use the locale of the environment
  initializeNcurses();

  enum Delay {
    DELAY_MIN = 33333,
    DELAY_MEDIUM = 50000,
    DELAY_MAX = 83333,
    DELAY_INCREMENT = DELAY_MAX - DELAY_MIN
  };

  // Instantiate the objects
  Screen *screen = newScreen();
  Snake *snake = newSnake((Point){screen->width / 2, screen->height / 2});

  if (!welcome(screen))
    goto QUIT;
  drawWalls(screen);

  Point collision = {-1, -1};

  spawnOrb(screen);

  float progress = 0.0;

  // GAME LOOP
  while (
      (insideBoundaries(screen, snake) && !selfCollision(snake, &collision)) ||
      gameOver(screen, snake, &collision, &progress)) {

    switch (getch()) { // Get keyboard input
    case 'w':
    case 'k':
    case KEY_UP:
      changeDirection(snake, NORTH);
      break;
    case 'l':
    case 'd':
    case KEY_RIGHT:
      changeDirection(snake, EAST);
      break;
    case 'j':
    case 's':
    case KEY_DOWN:
      changeDirection(snake, SOUTH);
      break;
    case 'h':
    case 'a':
    case KEY_LEFT:
      changeDirection(snake, WEST);
      break;
    case 'q':
      goto QUIT;
    }

    bool growing = false;
    Node *oldTail = advance(snake);
    if (snake->head->pos.x == screen->orb.x &&
        snake->head->pos.y == screen->orb.y) {
      growing = true;
      grow(snake, oldTail); // reappend oldTail to the Snake
      spawnOrb(screen);
      progress = (float)snake->length / (screen->width * screen->height);
    } else
      destroyNode(oldTail);

    draw(screen, snake, growing, oldTail);

    updateScore(screen, snake->length);

    switch (screen->difficulty) {
    case INCREMENTAL:
      usleep(DELAY_MAX - (unsigned)(DELAY_INCREMENT * progress));
      break;
    case EASY:
      usleep(DELAY_MAX); // 12 fps
      break;
    case MEDIUM:
      usleep(DELAY_MEDIUM); // 20 fps
      break;
    case HARD:
      usleep(DELAY_MIN); // 30 fps
      break;
    }
  }

QUIT:
  destroySnake(snake);
  destroyScreen(screen);
  endwin();
  return EXIT_SUCCESS;
}