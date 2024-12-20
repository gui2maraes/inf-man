#pragma once

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 600
#define INITIAL_HEALTH 3
#define PLAYER_SIZE 24
#define PLAYER_SPEED (PLAYER_SIZE * 4)
#define LEVEL_WIDTH 200
#define LEVEL_HEIGHT 10
#define TILE_SIZE 16
#define GRAVITY (25 * PLAYER_SIZE)
#define WORLDFILE_PATH "level.world"
#define GAME_SCALE ((float)1200 / (LEVEL_HEIGHT * TILE_SIZE * 2))
#define DEBUG_MODE (1)