#pragma once
#include "config.h"
#include "level.h"
#include "player.h"
#include "raylib.h"
#include "worldfile.h"

typedef struct Game {
  Level level;
  Player player;
  Camera2D camera;
} Game;

int Game_init(Game *game);

void Game_update(Game *game);
void Game_draw(Game *game);