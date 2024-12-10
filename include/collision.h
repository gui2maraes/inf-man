#pragma once
#include "config.h"
#include "level.h"
#include "raylib.h"

#include <stdint.h>

const uint8_t COLLISION_LEFT = 1;
const uint8_t COLLISION_RIGHT = 2;
const uint8_t COLLISION_UP = 4;
const uint8_t COLLISION_DOWN = 8;
typedef uint8_t CollisionMap;

/// Returns a bitmap containing the collisions of a hitbox
/// with all the blocks around it.
CollisionMap collision_check(Level *level, Rectangle hitbox,
                             Tile checked_tile) {
  CollisionMap collisions = 0;
  float x_offset = hitbox.width / 2.0;
  float y_offset = hitbox.height / 2.0;
  Vector2 points[8] = {
      // corners
      {hitbox.x - x_offset, hitbox.y - y_offset},
      {hitbox.x + x_offset, hitbox.y - y_offset},
      {hitbox.x + x_offset, hitbox.y + y_offset},
      {hitbox.x - x_offset, hitbox.y + y_offset},

      // midpoints
      {hitbox.x - x_offset, hitbox.y},
      {hitbox.x + x_offset, hitbox.y},
      {hitbox.x, hitbox.y - y_offset},
      {hitbox.x, hitbox.y + y_offset},
  };
  for (int i = 0; i < 8; ++i) {
    if (Level_is_tile(level, points[i], checked_tile)) {
    }
  }
}