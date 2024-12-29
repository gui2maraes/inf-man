#include "player.h"
#include "bullet.h"
#include "collision.h"
#include "config.h"
#include "game.h"
#include "level.h"
#include "raylib.h"
#include <stdio.h>

Rectangle Player_hitbox(Player *p, Tile tile) {
  float side = PLAYER_SIZE;
  if (tile == TILE_OBSTACLE) {
    side *= 0.8;
  }
  return (Rectangle){
      .x = p->pos.x, .y = p->pos.y, .width = side, .height = side};
}
bool Player_is_dead(Player *p) { return p->health <= 0; }

static Vector2 Player_feet(Player *p) {
  Vector2 translated = p->pos;
  translated.y += PLAYER_SIZE / 2.0;
  return translated;
}
static Vector2 Player_edge(Player *p, int x_sign) {
  Vector2 translated = p->pos;
  translated.x += (PLAYER_SIZE / 2.0) * x_sign;
  return translated;
}

void Player_damage(Player *p, Direction damage_direction) {
  if (p->invincible_time <= 0) {
    p->health -= 1;
    p->velocity.x = -damage_direction * PLAYER_DAMAGE_RECOIL * PLAYER_SIZE;
    p->velocity.y = PLAYER_DAMAGE_RECOIL * PLAYER_SIZE;
    p->invincible_time = PLAYER_INVINCIBILITY;
  }
}
static Direction collision_direction(Player *p, CollisionMap colls) {
  if (colls & COLLISION_RIGHT && !(colls & COLLISION_LEFT)) {
    return DIR_RIGHT;
  }
  if (colls & COLLISION_LEFT && !(colls & COLLISION_RIGHT)) {
    return DIR_LEFT;
  }
  return p->direction;
}

void Player_draw(Player *p) {
  Vector2 translated = p->pos;
  translated.x -= p->sprite.width / 2.0;
  translated.y -= p->sprite.height / 2.0;
  DrawTextureEx(p->sprite, translated, 0.0, 1.0, WHITE);
  if (DEBUG_MODE) {
    DrawCircleV(p->pos, PLAYER_SIZE / 10.0, RED);
    DrawCircleV(Player_feet(p), PLAYER_SIZE / 20.0, GREEN);
    DrawCircleV(Player_edge(p, -1), PLAYER_SIZE / 20.0, BLUE);
    DrawCircleV(Player_edge(p, 1), PLAYER_SIZE / 20.0, BLUE);
  }
}

void Player_update(Game *game, float delta) {
  Player *p = &game->player;
  Level *level = &game->level;
  //  if the player is out of level bounds, kill it.
  if (p->pos.y > LEVEL_HEIGHT * TILE_SIZE + PLAYER_SIZE) {
    p->health = 0;
  }
  if (Player_is_dead(p)) {
    return;
  }
  if (p->invincible_time > 0) {
    p->invincible_time -= delta;
  }
  if (IsKeyPressed(KEY_Z)) {
    Bullet_spawn(game);
  }
  // player horizontal movement
  p->velocity.x = 0;
  if (IsKeyDown(MOVE_LEFT_KEY)) {
    p->velocity.x += -PLAYER_SPEED * delta;
    p->direction = DIR_LEFT;
  }
  if (IsKeyDown(MOVE_RIGHT_KEY)) {
    p->velocity.x += PLAYER_SPEED * delta;
    p->direction = DIR_RIGHT;
  }
  p->pos.x += p->velocity.x;
  p->pos.y -= p->velocity.y * delta;

  CollisionMap collisions =
      resolve_collisions(level, Player_hitbox(p, TILE_BLOCK), TILE_BLOCK);
  Vector2 aligned = Level_align_coord_tile_center(p->pos);

  // if the player is colliding against a block, cancel the movement in that
  // direction.
  if (collisions & COLLISION_LEFT) {
    p->pos.x = aligned.x - TILE_SIZE / 2.0 + PLAYER_SIZE / 2.0;
  }
  if (collisions & COLLISION_RIGHT) {
    p->pos.x = aligned.x + TILE_SIZE / 2.0 - PLAYER_SIZE / 2.0;
  }
  if (collisions & COLLISION_UP) {
    p->pos.y = aligned.y - TILE_SIZE / 2.0 + PLAYER_SIZE / 2.0;
    p->velocity.y = 0;
  }
  // clamp player horizontally in level bounds
  if (p->pos.x < PLAYER_SIZE / 2.0) {
    p->pos.x = PLAYER_SIZE / 2.0;
  }

  // if the player is touching the floor
  if (collisions & COLLISION_DOWN) {
    p->pos.y = aligned.y + TILE_SIZE / 2.0 - PLAYER_SIZE / 2.0;
    p->velocity.y = 0;
    // and is jumping,
    if (IsKeyDown(JUMP_KEY) && !p->already_jumped) {
      p->velocity.y = PLAYER_SIZE * PLAYER_JUMP_FORCE;
      p->already_jumped = true;
    } else if (IsKeyUp(JUMP_KEY)) {
      p->already_jumped = false;
    }
  } else {
    p->velocity.y -= GRAVITY * delta;
    if (IsKeyReleased(JUMP_KEY) && p->velocity.y > 0) {
      p->velocity.y /= PLAYER_JUMP_DIVIDER;
    }
  }

  // if colliding with any obstacle, kill the player.
  CollisionMap obstacle_collisions =
      resolve_collisions(level, Player_hitbox(p, TILE_OBSTACLE), TILE_OBSTACLE);
  if (obstacle_collisions) {
    p->health = 0;
    // Player_damage(p, collision_direction(p, obstacle_collisions));
  }
}