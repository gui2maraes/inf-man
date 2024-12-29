#pragma once
#include "player.h"
#include "raylib.h"
#include <stdbool.h>

typedef struct Game Game;

typedef struct Bullet {
  Vector2 pos;
  float velocity;
  bool alive;
} Bullet;

/// Returns the Rectangle containing the bullet.
Rectangle Bullet_hitbox(Bullet *bullet);

/// Updates the bullet position and kill it when it leaves the screen.
void Bullet_update(Bullet *bullet, Player *p, float delta);
void Bullets_update(Game *game, float delta);

/// Draws a bullet if it's alive
void Bullet_draw(Bullet *bullet);
void Bullets_draw(Game *game);

/// Spawns a bullet if there is space.
void Bullet_spawn(Game *game);