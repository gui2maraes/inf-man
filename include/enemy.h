#pragma once
#include "animator.h"
#include "config.h"
#include "raylib.h"
#include "texmanager.h"
#include <stdbool.h>

typedef struct Game Game;

typedef struct Enemy {
  Vector2 pos;
  Vector2 velocity;
  bool alive;
  Direction dir;
} Enemy;

typedef struct EnemyManager {
  Enemy enemies[MAX_ENEMIES];
  Animator animator;
  int enemies_len;

} EnemyManager;

void EnemyManager_init(EnemyManager *enemy_mgr, TextureManager *tex);
void Enemy_spawn(EnemyManager *enemy_mgr, Vector2 pos);

/// Returns the enemy collision rectangle.
Rectangle Enemy_hitbox(Enemy *enemy);

void EnemyManager_update(Game *game, float delta);
void EnemyManager_draw(EnemyManager *enemy_mgr);