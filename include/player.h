#pragma once

#include "config.h"
#include "level.h"
#include "raylib.h"

typedef struct Player {
  Vector2 pos;
  Vector2 velocity;
  int health;
  int score;
  Texture sprite;

} Player;

/// Initializes player to 0 with initial health and sprite.
static inline void Player_init(Player *p, Texture sprite) {
  *p = (Player){0};
  p->health = INITIAL_HEALTH;
  p->sprite = sprite;
}
void Player_draw(Player *p);
int Player_is_dead(Player *p);
void Player_damage(Player *p);
void Player_update(Player *p, Level *level, float delta);
Rectangle Player_hitbox(Player *p, Tile tile);