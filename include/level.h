#pragma once
#include "collision.h"
#include "config.h"
#include "enemy.h"
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
  Tile tiles[LEVEL_WIDTH][LEVEL_HEIGHT];
  Vector2 spawn_point;
  Texture sprite;
  Vector2 sprite_offset;
} Level;

/// Initializes level with worldfile and generates world texture.
/// Also populate the enemies array.
/// returns 0 on failure and non-zero on success.
int Level_init(Level *level, EnemyManager *enemy_mgr, char *worldfile);
/// Transforms a line and column index into the level tile matrix
/// into 2D world coordinates.
Vector2 Level_matrix_to_world(int line, int column);
/// Transforms a world coordinate into the level tile matrix
/// coordinates. Returns true if inbounds or false if out of bounds.
int Level_world_to_matrix(Vector2 v, int *x, int *y);
void Level_gen_texture(Level *level);
void Level_draw(Level *level);

/// Checks if a world coordinate is a certain tile
int Level_coord_is_tile(Level *l, Vector2 v, Tile tile);

Vector2 Level_align_coord_tile_center(Vector2 coords);

/// Checks for collisions against a tile type. Returns
/// a bitmap containing the collision directions.
CollisionMap resolve_collisions(Level *level, Rectangle hitbox,
                                Tile checked_tile);