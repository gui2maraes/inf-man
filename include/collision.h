#pragma once

#include "level.h"
#include "player.h"
#include <stdint.h>

#define COL_PT_OFFSET 3

#define COLLISION_LEFT 1
#define COLLISION_RIGHT 2
#define COLLISION_UP 4
#define COLLISION_DOWN 8

typedef uint8_t CollisionMap;

/// Resolves all types of tile collisions and returns whether the player can
/// jump.
CollisionMap resolve_collisions(Level *level, Rectangle hitbox,
                                Tile checked_tile);
/// Returns a bitmap containing the collisions of a hitbox
/// with all the blocks around it.
// CollisionMap collision_check(Level *level, Rectangle hitbox,
//                              Tile checked_tile) {
//   CollisionMap collisions = 0;
//   float x_offset = hitbox.width / 2.0;
//   float y_offset = hitbox.height / 2.0;
//   Vector2 bottom_points[3] = {
//       {hitbox.x - x_offset, hitbox.y + y_offset + COL_PT_OFFSET},
//       {hitbox.x, hitbox.y + y_offset + COL_PT_OFFSET},
//       {hitbox.x + x_offset, hitbox.y + y_offset + COL_PT_OFFSET},
//   };
//   Vector2 top_points[3] = {
//       {hitbox.x - x_offset, hitbox.y - y_offset - COL_PT_OFFSET},
//       {hitbox.x, hitbox.y - y_offset - COL_PT_OFFSET},
//       {hitbox.x + x_offset, hitbox.y - y_offset - COL_PT_OFFSET},
//   };
//   Vector2 left_points[3] = {
//       {hitbox.x - x_offset - COL_PT_OFFSET, hitbox.y - y_offset},
//       {hitbox.x - x_offset - COL_PT_OFFSET, hitbox.y},
//       {hitbox.x - x_offset - COL_PT_OFFSET, hitbox.y + y_offset},
//   };
//   Vector2 right_points[3] = {
//       {hitbox.x + x_offset + COL_PT_OFFSET, hitbox.y - y_offset},
//       {hitbox.x + x_offset + COL_PT_OFFSET, hitbox.y},
//       {hitbox.x + x_offset + COL_PT_OFFSET, hitbox.y + y_offset},
//   };
//   Vector2 *sides[4] = {bottom_points, top_points, left_points, right_points};
//   uint8_t directions[4] = {COLLISION_DOWN, COLLISION_UP, COLLISION_LEFT,
//                            COLLISION_RIGHT};
//   for (int i = 0; i < 4; ++i) {
//     Vector2 *side = sides[i];
//     uint8_t dir = directions[i];
//     for (int pt = 0; pt < 3; ++pt) {
//       if (Level_is_tile(level, side[pt], checked_tile)) {
//         collisions |= dir;
//       }
//     }
//   }
//   return collisions;
// }