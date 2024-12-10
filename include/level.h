#pragma once
#include "raylib.h"

typedef enum Tile {
  TILE_SPACE,
  TILE_BLOCK,
  TILE_OBSTACLE,
  TILE_SPAWN,
} Tile;

/// All data needed for a level.
/// Initialized by reading a worldfile
typedef struct Level {
  Tile tiles[200][10];
  Vector2 spawn_point;
  Texture sprite;
} Level;

/// Initializes level with worldfile and generates world texture.
/// returns 0 on failure and non-zero on success.
int Level_init(Level *level, char *worldfile_path);
/// Transforms a line and column index into the level tile matrix
/// into 2D world coordinates.
Vector2 Level_matrix_to_world(int line, int column);
/// Transforms a world coordinate into the level tile matrix
/// coordinates. Returns true if inbounds or false if out of bounds.
int Level_world_to_matrix(Vector2 v, int *x, int *y);
void Level_gen_texture(Level *level);
void Level_draw(Level *level);

/// Checks if a world coordinate is a certain tile
int Level_is_tile(Level *l, Vector2 v, Tile tile);