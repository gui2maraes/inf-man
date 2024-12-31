#pragma once
#include "animator.h"
#include "collision.h"
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
  Vector2 spawn_positions[MAX_ENEMIES];
  Animator animator;
  int enemies_len;

} EnemyManager;

/// Initializes the EnemyManager
void EnemyManager_init(EnemyManager *enemy_mgr, TextureManager *tex);
/// Resets the enemies
void EnemyManager_spawn(EnemyManager *enemy_mgr);
/// Adds an enemy position for the next spawn
void EnemyManager_add(EnemyManager *enemy_mgr, Vector2 pos);

void EnemyManager_update(Game *game, float delta);
void EnemyManager_draw(EnemyManager *enemy_mgr);

/// Returns the enemy colliding with the provided hitbox, or NULL if no enemy
/// is colliding.
Enemy *EnemyManager_colliding_with(EnemyManager *enemy_mgr, Hitbox hitbox);