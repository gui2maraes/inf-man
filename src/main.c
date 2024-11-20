#include "game.h"
#include "raylib.h"
#include <stdio.h>

#define GAME_SCALE 4

int main(void) {
  InitWindow(1200, 600, "INF MAN");
  SetTargetFPS(60);

  Game game;

  if (!Game_init(&game)) {
    return 1;
  }
  while (!WindowShouldClose()) {
    Game_update(&game);
    Game_draw(&game);
  }
  CloseWindow();
  return 0;
}