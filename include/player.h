#pragma once

#include "config.h"
#include "leaderboard.h"
#include "level.h"
#include "raylib.h"
#include <stdbool.h>
typedef struct Game Game;

typedef enum Direction {
  DIR_LEFT = -1,
  DIR_RIGHT = 1,
} Direction;

typedef struct Player {
  Vector2 pos;
  Vector2 velocity;
  int health;
  PlayerRecord record;
  Texture sprite;
  Direction direction;
  bool already_jumped;
  float invincible_time;

} Player;

/// Initializes player to 0 with initial health and sprite.
static inline void Player_init(Player *p) {
  *p = (Player){0};
  p->health = INITIAL_HEALTH;
  p->sprite = LoadTexture("assets/man.png");
  p->direction = DIR_RIGHT;
}
/// Draws the player.
void Player_draw(Player *p);
/// Returns true if the player's health is 0, and false otherwise.
bool Player_is_dead(Player *p);

void Player_damage(Player *p, Direction damage_direction);
// void Player_update(Player *p, Level *level, float delta);
void Player_update(Game *game, float delta);
Rectangle Player_hitbox(Player *p, Tile tile);