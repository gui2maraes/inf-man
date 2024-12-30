#include "game.h"
#include "raylib.h"
#include "texmanager.h"

int main(void) {
  InitWindow(1200, 600, "INF MAN");
  SetTargetFPS(60);
  SetExitKey(KEY_NULL);

  Game game;

  if (!Game_init(&game)) {
    return 1;
  }
  while (!WindowShouldClose()) {
    if (!Game_update(&game)) {
      break;
    }
    Game_draw(&game);
  }
  CloseWindow();
  return 0;
}