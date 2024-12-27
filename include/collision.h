#pragma once

#include "level.h"
#include <stdint.h>

#define COL_PT_OFFSET 3

#define COLLISION_LEFT 1
#define COLLISION_RIGHT 2
#define COLLISION_UP 4
#define COLLISION_DOWN 8

typedef uint8_t CollisionMap;

/// Checks for collisions against a tile type. Returns
/// a bitmap containing the collision directions.
CollisionMap resolve_collisions(Level *level, Rectangle hitbox,
                                Tile checked_tile);