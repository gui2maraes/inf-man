#include "collision.h"
#include "config.h"
#include "level.h"
#include "raylib.h"
#include <math.h>

/// Checks for collisions against a tile type. Returns
/// a bitmap containing the collision directions.
CollisionMap resolve_collisions(Level *level, Rectangle hitbox,
                                Tile checked_tile) {
  CollisionMap colls = 0;
  float x_offset = hitbox.width / 2.0;
  float y_offset = hitbox.height / 2.0;
  Vector2 points[8] = {
      {hitbox.x - x_offset, hitbox.y - y_offset},
      {hitbox.x, hitbox.y - y_offset},
      {hitbox.x + x_offset, hitbox.y - y_offset},

      {hitbox.x - x_offset, hitbox.y},
      {hitbox.x + x_offset, hitbox.y},

      {hitbox.x - x_offset, hitbox.y + y_offset},
      {hitbox.x, hitbox.y + y_offset},
      {hitbox.x + x_offset, hitbox.y + y_offset},
  };
  for (int i = 0; i < 8; ++i) {
    if (Level_coord_is_tile(level, points[i], checked_tile)) {
      Vector2 aligned = Level_align_coord_tile_center(points[i]);
      Vector2 dist = {.x = hitbox.x - aligned.x, hitbox.y - aligned.y};
      if (fabsf(dist.x) > fabsf(dist.y)) {
        if (dist.x < 0.0) {
          colls |= COLLISION_RIGHT;
        } else {
          colls |= COLLISION_LEFT;
        }
      } else if (fabsf(dist.x) < TILE_SIZE - 0.5) {
        if (dist.y < 0.0) {
          colls |= COLLISION_DOWN;
        } else {
          colls |= COLLISION_UP;
        }
      }
    }
  }
  return colls;
}
