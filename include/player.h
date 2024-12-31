#pragma once

#include "animator.h"
#include "leaderboard.h"
#include "level.h"
#include "raylib.h"
#include "texmanager.h"
#include "utils.h"
#include <stdbool.h>

typedef struct Game Game;

typedef struct Player {
  Vector2 pos;
  Vector2 velocity;
  int health;
  PlayerRecord record;
  Direction direction;
  bool already_jumped;
  float invincible_time;
  float shooting_time;
  Animator animator;
  Animator shooting;

} Player;

typedef enum PlayerAnimationState {
  PLAYER_IDLE = 0,
  PLAYER_RUNNING = 1,
  PLAYER_JUMPING = 2,
  PLAYER_DAMAGE = 3,
} PlayerAnimationState;

/// Initializes player to 0 with initial health and sprite.
void Player_init(Player *p, TextureManager *tex);
/// Draws the player.
void Player_draw(Player *p);
/// Returns true if the player's health is 0, and false otherwise.
bool Player_is_dead(Player *p);

void Player_damage(Player *p, Direction damage_direction);
// void Player_update(Player *p, Level *level, float delta);
void Player_update(Game *game, float delta);
Rectangle Player_hitbox(Player *p, Tile tile);