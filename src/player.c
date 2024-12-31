#include "player.h"
#include "animator.h"
#include "bullet.h"
#include "collision.h"
#include "config.h"
#include "enemy.h"
#include "game.h"
#include "level.h"
#include "raylib.h"
#include <math.h>

static void Player_animator_init(Animator *animator, TextureManager *tex) {
  Animator_init(animator, tex->player);
  Animator_change(animator, PLAYER_IDLE);
  Animator_add(animator, PLAYER_IDLE,
               Animation_new((Rectangle){0, 0, 72, 24}, 3, 0.5));
  Animator_add(animator, PLAYER_RUNNING,
               Animation_new((Rectangle){0, 24, 72, 24}, 3, 0.1));
  Animator_add(animator, PLAYER_JUMPING,
               Animation_new((Rectangle){0, 96, 32, 32}, 1, 1));
}
static void Player_animator_shooting_init(Animator *animator,
                                          TextureManager *tex) {
  Animator_init(animator, tex->player);
  Animator_change(animator, PLAYER_IDLE);
  Animator_add(animator, PLAYER_IDLE,
               Animation_new((Rectangle){0, 48, 32, 24}, 1, 1));
  Animator_add(animator, PLAYER_RUNNING,
               Animation_new((Rectangle){0, 72, 96, 24}, 3, 0.1));
  Animator_add(animator, PLAYER_JUMPING,
               Animation_new((Rectangle){32, 96, 32, 32}, 1, 1));
}

/// Initializes player to 0 with initial health and sprite.
void Player_init(Player *p, TextureManager *tex) {
  *p = (Player){0};
  p->health = INITIAL_HEALTH;
  p->direction = DIR_RIGHT;
  Player_animator_init(&p->animator, tex);
  Player_animator_shooting_init(&p->shooting, tex);
}

Rectangle Player_hitbox(Player *p, Tile tile) {
  float side = PLAYER_SIZE;
  if (tile == TILE_OBSTACLE) {
    side *= 0.8;
  }
  return (Rectangle){
      .x = p->pos.x, .y = p->pos.y, .width = side, .height = side};
}
Hitbox Player_enemy_hitbox(Player *p) {
  return (Hitbox){p->pos, PLAYER_SIZE / 2.0};
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
  float inv = p->invincible_time;
  if (inv > 0 && (int)(inv * 10) % 2) {
    return;
  }
  Rectangle rect = Animator_current_sprite(&p->animator);
  if (p->shooting_time > 0.0) {
    Animator_draw(&p->shooting, p->pos, p->direction);
  } else {
    Animator_draw(&p->animator, p->pos, p->direction);
  }
  if (DEBUG_MODE) {
    Hitbox hb = Player_enemy_hitbox(p);
    DrawCircleV(hb.pos, hb.radius, RED);
  }
}

static void Player_collide_enemies(Game *game) {
  Player *p = &game->player;
  Rectangle p_hitbox = Player_hitbox(p, TILE_OBSTACLE);
}

void Player_update(Game *game, float delta) {
  Player *p = &game->player;
  Level *level = &game->level;
  EnemyManager *enemy_mgr = &game->enemy_mgr;
  Animator *anim = &p->animator;
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
  if (p->shooting_time > 0) {
    p->shooting_time -= delta;
  }
  if (IsKeyPressed(KEY_Z)) {
    Bullet_spawn(game);
    p->shooting_time = PLAYER_SHOOTING_TIME;
  }
  if (p->shooting_time > 0) {
    anim = &p->shooting;
  }
  // player horizontal movement
  if (p->invincible_time < PLAYER_INVINCIBILITY / 2.0) {
    p->velocity.x = 0;
    if (IsKeyDown(MOVE_LEFT_KEY)) {
      p->velocity.x += -PLAYER_SPEED;
      p->direction = DIR_LEFT;
    }
    if (IsKeyDown(MOVE_RIGHT_KEY)) {
      p->velocity.x += PLAYER_SPEED;
      p->direction = DIR_RIGHT;
    }
  }
  p->pos.x += p->velocity.x * delta;
  p->pos.y -= p->velocity.y * delta;

  // check collision with enemies
  if (p->invincible_time <= 0 &&
      EnemyManager_colliding_with(enemy_mgr, Player_enemy_hitbox(p))) {
    Player_damage(p, DIR_RIGHT);
  }

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
  if (obstacle_collisions && p->invincible_time <= 0) {
    p->health = 0;
    // Player_damage(p, collision_direction(p, obstacle_collisions));
  }

  // animation updating
  Animator_update(anim, delta);
  if (p->velocity.y != 0) {
    Animator_change(anim, PLAYER_JUMPING);
  } else if (fabsf(p->velocity.x) > 0.1) {
    Animator_change(anim, PLAYER_RUNNING);
  } else {
    Animator_change(anim, PLAYER_IDLE);
  }
}