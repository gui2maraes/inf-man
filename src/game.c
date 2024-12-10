#include "game.h"
#include "config.h"
#include "level.h"
#include "raymath.h"
#include <stdio.h>

/// Initializes all the game state.
/// Loads the worldfile, generates the level texture
/// and initializes player and enemies.
/// Returns 1 on success and 0 on failure
int Game_init(Game *game) {
  *game = (Game){0};
  game->camera.zoom = GAME_SCALE;
  game->camera.offset.x = WINDOW_WIDTH / 2.0;
  // game->camera.offset.y = WINDOW_HEIGHT / 2.0;
  if (!Level_init(&game->level, WORLDFILE_PATH)) {
    return 0;
  }
  Player_init(&game->player, LoadTexture("assets/man.png"));
  game->player.pos = game->level.spawn_point;

  printf("Spawn point: x: %f, y: %f", game->level.spawn_point.x,
         game->level.spawn_point.y);
  return 1;
}

/// Game logic function. Runs every frame.
void Game_update(Game *game) {
  float delta = GetFrameTime();
  Player_update(&game->player, &game->level, delta);
  game->camera.target.x = game->player.pos.x;
}

/// Draw everything in the game. Runs every frame.
void Game_draw(Game *game) {
  BeginDrawing();
  ClearBackground(WHITE);

  BeginMode2D(game->camera);
  Level_draw(&game->level);
  Player_draw(&game->player);
  EndMode2D();

  EndDrawing();
}
