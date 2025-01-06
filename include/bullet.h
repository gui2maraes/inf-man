#pragma once
#include "collision.h"
#include "enemy.h"
#include "player.h"
#include "raylib.h"
#include <stdbool.h>

typedef struct Game Game;

typedef struct Bullet {
  Vector2 pos;
  float velocity;
  float alive_time;
} Bullet;

/// Returns true if the bullet is alive
bool Bullet_alive(Bullet *bullet);
/// Returns the Rectangle containing the bullet.
Hitbox Bullet_hitbox(Bullet *bullet);

/// Updates the bullet position and kill it when it leaves the screen.
void Bullet_update(Bullet *bullet, Player *p, EnemyManager *enemy_mgr,
                   float delta);
void Bullets_update(Game *game, float delta);

/// Draws a bullet if it's alive
void Bullet_draw(Bullet *bullet);
void Bullets_draw(Game *game);

/// Spawns a bullet if there is space.
void Bullet_spawn(Game *game);