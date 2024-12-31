#pragma once

#include "raylib.h"
#include <stdint.h>

#define COL_PT_OFFSET 3

#define COLLISION_LEFT 1
#define COLLISION_RIGHT 2
#define COLLISION_UP 4
#define COLLISION_DOWN 8

typedef uint8_t CollisionMap;
typedef struct Hitbox {
  Vector2 pos;
  float radius;
} Hitbox;
