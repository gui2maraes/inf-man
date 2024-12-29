#pragma once
#include "raylib.h"
#include <stdbool.h>

typedef struct Enemy {
  Vector2 pos;
  Vector2 velocity;
  bool alive;
} Enemy;

/// Returns the enemy collision rectangle.
Rectangle Enemy_hitbox(Enemy *enemy);