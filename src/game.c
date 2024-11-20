#include "game.h"
#include "config.h"

/// Initializes all the game state.
/// Loads the worldfile, generates the level texture
/// and initializes player and enemies.
/// Returns 1 on success and 0 on failure
int Game_init(Game *game) {
  *game = (Game){0};
  game->camera.zoom = GAME_SCALE;
  if (!Level_init(&game->level, WORLDFILE_PATH)) {
    return 0;
  }
  Player_init(&game->player, LoadTexture("assets/man.png"));
  return 1;
}

/// Game logic function. Runs every frame.
void Game_update(Game *game) {
  float delta = GetFrameTime();
  Player_update(&game->player, &game->level, delta);
}

/// Draw everything in the game. Runs every frame.
void Game_draw(Game *game) {
  BeginDrawing();
  ClearBackground(WHITE);

  Player_draw(&game->player);

  EndDrawing();
}