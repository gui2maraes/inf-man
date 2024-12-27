#include "game.h"
#include "config.h"
#include "gui.h"
#include "level.h"
#include "raylib.h"
#include "utils.h"
#include <stdio.h>

/// Initializes all the game state.
/// Loads the worldfile, generates the level texture
/// and initializes player and enemies.
/// Returns 1 on success and 0 on failure
int Game_init(Game *game) {
  *game = (Game){0};
  game->state = GAME_TITLE_SCREEN;
  game->camera.zoom = GAME_SCALE;
  game->camera.offset.x = WINDOW_WIDTH / 2.0;
  // game->camera.offset.y = WINDOW_HEIGHT / 2.0;
  if (!Level_init(&game->level, WORLDFILE_PATH)) {
    return 0;
  }
  Player_init(&game->player, LoadTexture("assets/man.png"));
  game->player.pos = game->level.spawn_point;

  game->title_sprite = LoadTexture("assets/inf_man.png");
  printf("Spawn point: x: %f, y: %f", game->level.spawn_point.x,
         game->level.spawn_point.y);
  if (!Leaderboard_init(&game->leaderboard, LEADERBOARD_FILE)) {
    return 0;
  }
  return 1;
}

/// Draws the Game title screen
static void Game_menu_draw(Game *game) {
  int half_width = WINDOW_WIDTH / 2;
  int title_y = WINDOW_HEIGHT / 3 - game->title_sprite.height / 2;
  int next_y = title_y + game->title_sprite.height / 2;
  int step = GUI_FONT_SIZE + GUI_PADDING;

  BeginDrawing();
  ClearBackground(BLUE);
  DrawTexture(game->title_sprite, half_width - game->title_sprite.width / 2,
              title_y, WHITE);
  if (gui_button("Start", half_width, next_y + step * 2)) {
    game->state = GAME_RUNNING;
  }
  if (gui_button("Leaderboard", half_width, next_y + step * 3)) {
    game->state = GAME_LEADERBOARD;
  }
  if (gui_button("Exit", half_width, next_y + step * 4)) {
    game->state = GAME_CLOSE;
  }

  EndDrawing();
}

static void Game_leaderboard_draw(Game *game) {
  char entry[PLAYER_NAME_MAX + 5] = {0};
  int half_width = WINDOW_WIDTH / 2;
  int next_y = WINDOW_HEIGHT / 8;
  BeginDrawing();
  ClearBackground(BLUE);
  gui_text("LEADERBOARD", half_width, next_y, 1.5, ALIGN_CENTER);
  next_y += GUI_FONT_SIZE * 3;
  for (int i = 0; i < LEADERBOARD_NUM; ++i) {
    PlayerRecord *p = &game->leaderboard.players[i];
    /// Write information to a string
    snprintf(entry, PLAYER_NAME_MAX + 5, "%s", p->name);
    gui_text(entry, half_width, next_y, 1.0, ALIGN_RIGHT);
    snprintf(entry, PLAYER_NAME_MAX + 5, " - %d", p->score);
    gui_text(entry, half_width, next_y, 1.0, ALIGN_LEFT);
    next_y += GUI_FONT_SIZE + GUI_PADDING;
  }
  next_y += GUI_FONT_SIZE;
  if (gui_button("Back", half_width, next_y)) {
    game->state = GAME_TITLE_SCREEN;
  }
  EndDrawing();
}

/// Game logic function. Runs every frame.
static void Game_running_update(Game *game) {
  float delta = GetFrameTime();
  Player_update(&game->player, &game->level, delta);
  game->camera.target.x = game->player.pos.x;
}

/// Draw everything in the game. Runs every frame.
static void Game_running_draw(Game *game) {
  BeginDrawing();
  ClearBackground(WHITE);

  BeginMode2D(game->camera);
  Level_draw(&game->level);
  Player_draw(&game->player);
  EndMode2D();

  EndDrawing();
}

/// Draws the game depending on its game state.
void Game_draw(Game *game) {
  switch (game->state) {
  case GAME_RUNNING:
    Game_running_draw(game);
    break;
  case GAME_TITLE_SCREEN:
    Game_menu_draw(game);
    break;
  case GAME_LEADERBOARD:
    Game_leaderboard_draw(game);
    break;
  default:
    error_out("not implemented");
  }
}

/// Updates the Game structure.
/// Returns 1 if the game should keep running
/// and 0 if the game should close.
int Game_update(Game *game) {
  switch (game->state) {
  case GAME_RUNNING:
    Game_running_update(game);
    return 1;
    break;
  case GAME_CLOSE:
    return 0;
  default:
    return 1;
  }
}