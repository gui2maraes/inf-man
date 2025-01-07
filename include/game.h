#pragma once
#include "bullet.h"
#include "config.h"
#include "enemy.h"
#include "gui.h"
#include "leaderboard.h"
#include "level.h"
#include "player.h"
#include "raylib.h"
#include "texmanager.h"

typedef enum GameState {
  GAME_TITLE_SCREEN,
  GAME_LEADERBOARD,
  GAME_PAUSED,
  GAME_RUNNING,
  GAME_DIED,
  GAME_WON,
  GAME_CLOSE,
} GameState;

typedef struct Game {
  GameState state;
  Level level;
  Player player;
  Bullet bullets[MAX_BULLETS];
  EnemyManager enemy_mgr;
  Camera2D camera;
  Leaderboard leaderboard;
  TextureManager tex;
} Game;

/// Initializes a Game struct.
/// Returns true on success and false on failure.
bool Game_init(Game *game);

/// Updates the game
bool Game_update(Game *game);
/// Draws the game
void Game_draw(Game *game);

void Game_change_state(Game *game, GameState new_state);