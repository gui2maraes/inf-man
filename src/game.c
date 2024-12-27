#include "game.h"
#include "config.h"
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
  return 1;
}

static int menu_button(char *label, int x, int y) {
  int text_size = MeasureText(label, MENU_FONT_SIZE);
  Rectangle rect = {x - text_size / 2.0, y, text_size, MENU_FONT_SIZE};
  Vector2 mouse_pos = GetMousePosition();
  Color col = WHITE;
  int out = 0;
  if (CheckCollisionPointRec(mouse_pos, rect)) {
    col = LIGHTGRAY;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      out = 1;
    }
  }
  DrawText(label, rect.x, rect.y, MENU_FONT_SIZE, col);
  return out;
}

static void Game_menu_draw(Game *game) {
  int half_width = WINDOW_WIDTH / 2;
  int title_y = WINDOW_HEIGHT / 3 - game->title_sprite.height / 2;
  int next_y = title_y + game->title_sprite.height / 2;
  int step = MENU_FONT_SIZE + MENU_PADDING;

  BeginDrawing();
  ClearBackground(BLUE);
  DrawTexture(game->title_sprite, half_width - game->title_sprite.width / 2,
              title_y, WHITE);
  if (menu_button("Start", half_width, next_y + step * 2)) {
    game->state = GAME_RUNNING;
  }
  if (menu_button("Leaderboard", half_width, next_y + step * 3)) {
    game->state = GAME_LEADERBOARD;
  }
  if (menu_button("Exit", half_width, next_y + step * 4)) {
    game->state = GAME_CLOSE;
  }

  EndDrawing();
}
static void Game_menu_update(Game *game) {}

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

void Game_draw(Game *game) {
  switch (game->state) {
  case GAME_TITLE_SCREEN:
    Game_menu_draw(game);
    break;
  case GAME_RUNNING:
    Game_running_draw(game);
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